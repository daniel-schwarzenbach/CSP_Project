
#include <Metropolis/metropolis.h++>
#include <Metropolis/adaptive_metropolis.h++>
#include <Wolff/wolff.h++>

#include <matplot/matplot.h>
#include <Heisenberg.h++>
#include <Timekeeper.h++>
#include <Observables.h++>

/*
This is the main file which we run to get all calculations for the project.
*/

int main(int mainArgCount, char** mainArgs){

 


    /*
    Code for the Metropolis, Metropolis Advanced, and Wolff algorithms:
    Input: Reference to Lattice& lattice, temperature (T)
    Output: Nothing (Metropolis, Metropolis Advanced), flt susceptibility (Wolff algorithm)
    */
   
    /*
    All algorithm functions (X = Metropolis, Metropolis Advanced, Wolff) have to be defined as following:
    void / float X_algorithm(Lattice& lattice, flt T, flt time, int steps) { ... }

    - lattice: reference to the Lattice (which is changed after one run of the respective algorithm)
    - T: temperature which is used to calculate the thermodynamic beta (with the function Beta(flt T))
    - time: time in [sec.] after which the algorithm stops; if time = 0 => the algorithm is stopped after a certain no. of steps
    - steps: steps in [#] after which the algorithm stops; if steps = 0 => the algorithm is stopped after a certain time
        -   if time = 0 and steps = 0 => the algorithm does nothing / returns 0
        -   if time ≠ 0 and steps ≠ 0 => the algorithm stops after time [sec.] or steps [#], whatever occurs earlier

    Notice that the same no. of steps leads to a different run time for different algorithms. 

    The algorithm function return:
    - void (Metropolis, Metropolis Advanced) 
    - float (Wolff)
    */

    void Metropolis_algorithm(Lattice& lattice, flt T, flt time, int steps, flt J) {
        // Here, the Metropolis algorithm acts on a given lattice and changes the lattice
    };

    void Metropolis_advanced_algorithm(Lattice& lattice, flt T, flt time, int steps, flt J) {
        // Here, the Metropolis advanced algorithm acts on a given lattice and changes the lattice
    };

    flt Wolff_algorithm(Lattice& lattice, flt T, flt time, flt steps) {
        susceptibility = wolff(lattice, T, time, steps); // Here, the Metropolis algorithm acts on a given lattice and changes the lattice
        return susceptibility;
    };

    /*
    Define the parameters of the lattice:
    lattice sizes (Lx, Ly, Lz), interaction strength J, external magnetic field vector h_vector
    boundary coundtions (BC: o for open, p for periodic), 
    temperature (T), boundary coundtions (BC: o for open, p for periodic)
    */
    flt J = 1;
    Vector3 h_vector = {2.0, 1.0, 0.0};

    int Lx = 2;
    int Ly = 2;
    int Lz = 2;
    flt T = 1;
    flt beta = Beta(T);
    BC boundaries = BC::_0;

    Lattice lattice(Lx,Ly,Lz,boundaries);

    // This definition of lattice parameters is done ONCE for every experiment

    /*
    Define the Monte Carlo calculation for the expectation values
    <A> = lim N->∞ 1/N * 𝚺^N_{i = 1} A_i
    where we use approximate expectation values for a fixed N of runs:
    <A>_N = 1/N * 𝚺^N_{i = 1} A_i

    Here, A_i = M_i,E_i (magnetization vector, energy) are defined by calculating
    the respective quantities given a certain lattice configuration i. 

    In the first step (i=0) for A_0, we calculate the observable A for
    an initial lattice configuration in which for
    - T < T_C:  ... all spins point in +z direction,
                i. e. lattice(x,y,z) = (0,0,1) ∀ x∈Lx,y∈Ly,z∈Lz,
    - T > T_C:  ... all spins have a random orientiation.
    The case of T = T_C is tested by approaching T_C in both the disordered
    and ordered phases. 
    
    If we are in the high-T_C phase (high_t_c_phase = true),
    then the initial lattice has spins with random orientation at every site.
    Otherwise, all spins point in +z-direction in the low-T_C phase (high_t_c_phase = false).
    */

    int N = 100; // given 
    
    bool high_t_c_phase = true; // given

    if high_t_c_phase {
        // set lattice with spins in random orientation at every site
    } else {
        // set lattice with every spin in (0,0,1)
        lattice = Lattice::cons
    }


    //For all of the following four functions to calculate
    //<M>,<M^2>,<E>,<E^2>,
    //we have defined a fixed time in seconds or no. of steps after which each algorithms is finished

    N_STEPS_METROPOLIS = 10000;
    N_STEPS_METROPOLIS_AD = 5000;
    N_STEPS_WOLFF = 1000;

    T_TIME_METROPOLIS = 0;
    T_TIME_METROPOLIS_AD = 0;
    T_TIME_WOLFF = 0;

    /*
    Return the average magnetizaion after N runs of a certain algorithm algorithm = M,Mad,W
    */
    vector<flt> avg_magnetization(algorithm = M,Mad,W, N) {
        //define initial value (e. g. int for E, vector for M)
        vector<flt> M(3) = 0

        // calculate the expectation value after N runs:
        for (int i = 0; i < N; i++){
            //calculate A with a given algorithm:
            //- call algortihm function with lattice reference
            algorithm(Lattice& lattice, flt T, flt T_TIME_algo, int N_STEPS_algo)

            //- calculate M, E for the new lattice configuration
            M(3) += get_magnetization(const Lattice& lattice)
            //repeat this calculation in the next step of the for-loop with the changed lattice
        }

        //divide A by the number of runs
        M(3) *= 1/N;

        return M(3);
    };

    flt avg_magnetization_squared(algorithm = M,Mad,W, N) {
        flt M_squared = 0
        /*
        -"-
        but we calculate 
                M_squared += pow(get_magnetization(const Lattice& lattice), 2)
        */

        return M_squared;
    };

    /*
    Same two functions for the energy_
    */
    flt avg_energy(algorithm = M,Mad,W, N) {
        //define initial value (e. g. int for E, vector for M)
        flt E = 0;

        //...

        return E;
    };

    flt avg_energy_squared(algorithm = M,Mad,W, N) {
        //define initial value (e. g. int for E, vector for M)
        flt E_squared = 0;

        //...

        return E_squared;
    };


    /*
    This function tests the convergence of the different algorithms.
    We provide the following input values:
    -   observable (magnetization, energy)
    -   algorithm (M, Madvanced, Wolff)
    -   time (how long the algorithm runs), if time = 0 => we let the algorithm run for steps
    -   steps (how many steps each algorithm takes), if n_steps = 0 => we let the algorithm run for time seconds
            if time = 0 = steps => does nothing
            if time ≠ 0 ≠ steps => error
    -   n_variance (variance (and mean value) of the last n_variance steps are calculated)
    
    The function returns:
    -   observables_array, variance_array (arrays of length length_of_variance_array~steps/n_variance or ~time/n_variance)
    -   x_unit_array (array of same length as variance_array)
            This array contains values [1*n_variance, 2*n_variance, ..., length_of_variance_array*n_variance]
            which can be used as an x-array. The y-array of same length is then given by observables_array and variance_array.
    -   algorithm_time (how long the algorithm ran)
    -   algorithm_steps (how many steps the algorithm took)
    */

   // start with appropriate lattice

    vector<tuple<array<flt, 3>, flt, int>> convergence_of_algorithms(observable = M,E, algorithm = M,Mad,W, flt time, int steps, int n_variance) {
        flt algorithm_time = 0;
        int algorithm_steps = 0;

        /*
            if time = 0 = steps => do not run algorithm,
            if time ≠ 0 ≠ steps => do not run algorithm,
            -> return empty arrays and zero values
        */

        // implement error messages:

        /*
        for given no of steps: steps > n_variance, otherwise we output an error
        for given duration of seconds: time > n_variance/min_value, where min_value is the minimal time unit that we can measure
        /*

        // implement error messages:

        /*
        parameter = time, steps,
        */

        if (time != 0) {
            flt parameter = time;
        } else {
            int parameter = steps;
        };

        int length_of_variance_array = round(parameter/n_variance);

        /*
        n_variance is the time / no of steps during which the variance is calculated.
        In the loop, we start with variance_index = 0 and increase variance_index by 1 for every step.
        When variance_index = n_variance is reached, we calculate the variance over the last n_variance steps,
        save this variance in variance_array[variance_index], reset variance_index = 0 and count again. 
        This process is controlled by an outer loop which repeats this procedure length_of_variance_array times.
        */

        //initialize the array of mean values for the last variance_counter seconds or steps
        flt observables_array[length_of_variance_array];
        //initialize the array of variances for the last variance_counter seconds or steps
        flt variance_array[length_of_variance_array];

        for (int i = 0; i < length_of_variance_array, i++) {
            int variance_index = 0;

            //initialize the array of observable values (length is variance_counter)
            //of which the variance and mean is calculated
            flt current_values_array[variance_counter];


            /*
            THIS PART HERE IS NOT NECESSARY:
                when calling the algorithm functions, we can always input n_steps, t_time
            */

            while(variance_index < variance_counter) {

                algorithm()
                current_observable_value = // call the function
                current_values_array[variance_index] = current_observable_value
                variance_index++;
            }

            current_mean = average(current_values_array, variance_counter); //current mean
            current_variance = variance(current_values_array, variance_counter, current_mean); // current variance
            observables_array[i] = current_mean
            variance_array[i] = current_variance
        }


        //observables_array, variance_array, x_unit_array, algorithm_time, algorithm_steps;

        return result
    };


    

    cout << lattice.memory_size();

    Metropolis_advanced_algorithm(lattice, T, time, steps);
    
    

    cout << lattice.memory_size();
    cout << "hello world";
    return 0;
}