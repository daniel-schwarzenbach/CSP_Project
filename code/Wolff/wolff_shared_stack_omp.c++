#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>
#include <thread>
#include <random>
#include <shared_mutex>
#include <atomic>
#include <memory>

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
class SharedStack
{
private:
    struct Node
    {
        SharedPtr<T> data;
        Node *next;

        Node(T const &data_) : data(std::make_shared<T>(data_)) {}
    };

    std::atomic<Node *> head;

public:
    void push(T const &data)
    {
        Node *const new_node = new Node(data);
        new_node->next = head.load(std::memory_order_relaxed);
        while (!head.compare_exchange_weak(new_node->next, new_node,
                                           std::memory_order_release,
                                           std::memory_order_relaxed))
            ;
    }

    SharedPtr<T> pop()
    {
        Node *old_head = head.load(std::memory_order_relaxed);
        while (old_head && !head.compare_exchange_weak(
                               old_head, old_head->next,
                               std::memory_order_release,
                               std::memory_order_relaxed))
            ;
        return old_head ? old_head->data : SharedPtr<T>();
    }
};

// Function to flip the spin
inline void flip_spin(Spin const &randomSpin, Spin &startSpin)
{
    flt cdot = startSpin | randomSpin;
    startSpin = startSpin - (2.0 * cdot) * randomSpin;
}

inline flt activate_bond(flt const &J, Spin const &spinX,
                         Spin const &randomSpin,
                         flt const &beta, Spin const &spinY)
{
    flt energy = J * 2.0 * beta * (randomSpin | spinX) 
            * (randomSpin | spinY);
    return 1.0 - exp(min(energy, 0.0));
}

inline u64 wolf_algorithm_shared_stack_omp(Lattice &lattice, 
        flt const &beta, flt const &J, Array<Spin> &spinArray)
{
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    // Create vector that checks whether the site has been checked
    Lattice3D<u8> visited =
        Lattice3D<u8>::constant_lattice(Lx, Lz, Lz, false);
    visited.set_boundary_conditions(
        lattice.get_boundary_conditions());
    visited.set_zero_element(true);
    Array<u8> u8Array = visited.get_raw_data();

    SharedStack<Index> stack;
    u64 clusterSize = 0;

    Spin randomSpin = Spin::get_random();

    // Choose random lattice site as first point of cluster
    int x = rng::rand_int_range(0, Lx);
    int y = rng::rand_int_range(0, Ly);
    int z = rng::rand_int_range(0, Lz);

    stack.push({x, y, z});
    ++clusterSize;
    visited(x, y, z) = true;

    // Define spin_first to be flipped, first point of the cluster
    Spin &firstSpin = lattice(x, y, z);

    // Flip spin_first and mark Index {x,y,z}
    flip_spin(randomSpin, firstSpin);

    uint seed = rng::rand_int_range(0, 4'000'000);
    int threads_working = 0;

    #pragma omp parallel
    { // parallel
        // get the thread number
        const uint thread_number = omp_get_thread_num();
        // init random number generator for every thread
        std::mt19937 rndmgen(seed + thread_number);
        std::uniform_real_distribution<flt> uniform(0.0, 1.0);
        bool workToDo = true;
        if (omp_get_thread_num() != 0)
        {
            // wait 100ns
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(
                    2000ns + 100ns * thread_number);
        }
        while (workToDo)
        {
            SharedPtr<Index> currentPtr = stack.pop();
            // handle empty stack case
            if (currentPtr == nullptr)
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(
                        1000ns);
                int work;
                #pragma omp atomic read
                work = threads_working;
                if (work > 0)
                {
                    workToDo = true;
                }
                else
                {
                    workToDo = false;
                }
                continue;
            }
            // non empty stack case
            else
            {
                #pragma omp atomic update
                ++threads_working;

                Index current = *currentPtr;
                // Get current lattice position
                int x = current[0];
                int y = current[1];
                int z = current[2];

                uint lid = lattice.get_raw_id(current);
                Spin currentSpin;
                #pragma omp atomic read
                currentSpin[0] = spinArray[lid][0];
#               pragma omp atomic read
                currentSpin[1] = spinArray[lid][1];
                #pragma omp atomic read
                currentSpin[2] = spinArray[lid][2];

                Array<Index> neighbors = {
                        {x - 1, y, z}, {x, y - 1, z}, {x, y, z - 1}, 
                        {x + 1, y, z}, {x, y + 1, z}, {x, y, z + 1}};
                for (Index neighbor : neighbors)
                {
                    u8 wasVisited;
                    uint nid = lattice.get_raw_id(neighbor);
                    #pragma omp atomic read
                    wasVisited = u8Array[nid];

                    if (!wasVisited)
                    {
                        Spin neighborSpin;
                        #pragma omp atomic read
                        neighborSpin[0] = spinArray[nid][0];
                        #pragma omp atomic read
                        neighborSpin[1] = spinArray[nid][1];
                        #pragma omp atomic read
                        neighborSpin[2] = spinArray[nid][2];

                        // If Bond is activated...
                        if (uniform(rndmgen) <= 
                                activate_bond(J, currentSpin,
                                randomSpin, beta, neighborSpin))
                        {

                            flip_spin(randomSpin, neighborSpin);
                            #pragma omp atomic write
                            spinArray[nid][0] = neighborSpin[0];
                            #pragma omp atomic write
                            spinArray[nid][1] = neighborSpin[1];
                            #pragma omp atomic write
                            spinArray[nid][2] = neighborSpin[2];

                            stack.push(neighbor);

                            #pragma omp atomic write
                            u8Array[nid] = true;

                            #pragma omp atomic update
                            ++clusterSize;
                        } // end if activated
                    } // end if !wasVisited
                } // end for neighbors
                #pragma omp atomic update
                --threads_working;
            }
        } // end while(work to do)
    } // end parallel
    return clusterSize;
}

/*
performs the wolff algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

flt wolff_shared_stack_omp(Lattice &lattice, flt const &T, flt const &J,
              flt const &MaxTime, u64 const &MaxSteps)
{
    // to implement
    flt beta = Beta(T);
    measure::Timer watch;
    Array<flt> clusters(0);
    Array<Spin> &spinArray = lattice.get_raw_data();
    u64 nRuns = 0;

    for (; nRuns <= MaxSteps; ++nRuns)
    {
        wolf_algorithm_shared_stack_omp(lattice, beta, J, spinArray);
        clusters.push_back(0);
        // Check if maximum time has been reached
        if (watch.time() >= MaxTime)
        {
            break; // Stop simulation if maximum time reached
        }
    }
    return mean(clusters);
}