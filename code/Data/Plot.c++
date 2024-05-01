#include <Data/Plot.h++>
#include <Measure/Observables.h++>
#include <matplot/matplot.h>
#include <tuple>

namespace plt = matplot;
using std::to_string;

static Array<Array<double>> rainbow_dark{
    {0.0, 0.0, 0.9},
    {0.0, 0.6, 0.6},
    {0.0, 0.9, 0.0},
    {0.6, 0.6, 0.0},
    {0.9, 0.0, 0.0},
    {0.6, 0.0, 0.6},
    {0.0, 0.0, 0.9}};

static Array<Array<double>> heat{
    {0.6, 0.0, 0.0},
    {0.8, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.1, 0.0},
    {1.0, 0.3, 0.0},
    {1.0, 0.5, 0},
    {1.0, 0.9, 0},

    {1.0, 1.0, 0.5},

    {0.9, 1.0, 0.0},
    {0.5, 1.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.95, 0.4},
    {0.0, 0.9, 0.7},
    {0.0, 0.4, 0.7},
    {0.0, 0.0, 0.6}};

Array<Array<f32>> lattice_slice(Lattice &lattice, uint z)
{
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    Array<Array<f32>> latticeSlice(Lx,
                                   Array<f32>(Ly, 0));
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            Spin s = lattice(x, y, z);
            latticeSlice[x][y] = s.phi();
        }
    }
    return latticeSlice;
}

StaticArray<Array<f64>, 7> lattice_Arrays(Lattice &lattice)
{
    // get lattice size
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    Vector3 mag = measure::get_magnetization(lattice);
    mag.normalize();
    uint sizeL = Lx * Ly * Lz + 2;
    // initialize Arrays
    Array<double> x(sizeL, 0), y(sizeL, 0), z(sizeL, 0),
        u(sizeL, 0), v(sizeL, 0), w(sizeL, 0),
        color(sizeL, 0);
    uint i = 0;
#pragma omp parallel for
    for (uint ix = 0; ix < Lx; ++ix)
    {
        for (uint iy = 0; iy < Ly; ++iy)
        {
            for (uint iz = 0; iz < Lz; ++iz)
            {
                x[i] = ix;
                y[i] = iy;
                z[i] = iz;
                Spin s;
    #pragma omp critical
                s = lattice(ix, iy, iz);
                u[i] = s.x();
                v[i] = s.y();
                w[i] = s.z();

                
                // color the spins according to the magnetization
                color[i] = (s | mag);
#pragma omp critical
                i++;
            }
        }
    }
    x[i] = 0;
    y[i] = 0;
    z[i] = 0;
    u[i] = 0;
    v[i] = 0;
    w[i] = 0;
    color[i] = -1;
    ++i;
    x[i] = 0;
    y[i] = 0;
    z[i] = 0;
    u[i] = 0;
    v[i] = 0;
    w[i] = 0;
    color[i] = 1;
    ++i;
    return StaticArray<Array<double>, 7>{x, y, z, u, v, w, color};
    
}

bool data::plot_lattice_slice(Lattice &lattice, int z, std::string filename)
{

    Array<Array<f32>> latticeSlice = lattice_slice(lattice, z);

    auto fig = plt::figure(true);
    fig->size(1000, 1000);
    plt::colormap(rainbow_dark);
    plt::imagesc(latticeSlice);
    plt::caxis({0, _2pi_});
    plt::figure()->size(1000, 1000);
    if (filename != "")
    {
        plt::save(filename);
    }
    else
    {
        plt::show();
    }
    return 0;
}

bool data::plot_lattice(Lattice &lattice, std::string filename)
{
    StaticArray<Array<double>, 7> arrays = lattice_Arrays(lattice);
    // calculates average
    Vector3 mag =measure::get_magnetization(lattice);

    auto fig = plt::figure(true);
    fig->size(1000, 1000);
    string title = "Magnetization: {"+ to_str(mag(0)) + ", " 
                + to_str(mag(1)) + ", " + to_str(mag(2)) + " }";
    fig->title(title);
    plt::colormap(heat);
    plt::colorbar(true);
    plt::gca()->cblabel("Magnetization");
    plt::gca()->cb_position({1.1f, 0.f, 0.03f, 1.f});
    plt::gca()->position({0.135f, 0.1f, 0.7f, 0.85f});

    auto plot = plt::quiver3(arrays[0], arrays[1], arrays[2],
                             arrays[3],
                             arrays[4], arrays[5], arrays[6], 0.5);
    plot->line_width(3);
    plot->normalize(true);
    plt::caxis({1, -1});
    plt::xlabel("x");
    plt::ylabel("y");
    plt::zlabel("z");

    if (filename != "")
    {
        plt::save(filename);
    }
    else
    {
        plt::show();
    }
    fig.reset();
    plt::figure(false);
    return 0;
}

bool data::convert_pngs_to_gif(string gifname, string pngfilePrefix)
{
    double delay = 5;
    string command =
        "convert -delay " + to_string(delay) + " -layers Optimize " + pngfilePrefix + "*.png " + gifname;
    int i = std::system(command.c_str());
    return i;
}

