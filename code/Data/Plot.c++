#include <Data/Plot.h++>
#include <Measure/Observables.h++>
#include <matplot/matplot.h>
#include <tuple>

namespace plt = matplot;
using std::to_string;

static Array<Array<double>> rainbow_dark {
    {0.0, 0.0, 0.9},
    {0.0, 0.6, 0.6},
    {0.0, 0.9, 0.0},
    {0.6, 0.6, 0.0},
    {0.9, 0.0, 0.0},
    {0.6, 0.0, 0.6},
    {0.0, 0.0, 0.9}
};

static Array<Array<double>> heat {
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
    {0.0, 0.0, 0.6}
};

Array<Array<flt>> lattice_slice(Lattice &lattice, uint z)
{
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    Array<Array<flt>> latticeSlice(Lx,
                                   Array<flt>(Ly, 0));
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

StaticArray<Array<F64>, 7> lattice_Arrays(Lattice &lattice)
{
    // get lattice size
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    uint sizeL = Lx * Ly * Lz +2;
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
                Spin s = lattice(ix, iy, iz);
                u[i] = s.x();
                v[i] = s.y();
                w[i] = s.z();
                Vector3 mag;
                #pragma omp critical
                {
                mag = measure::get_magnetization(lattice);
                }
                // color the spins according to the magnetization
                color[i] = (s|mag);
                #pragma omp critical
                i++;
            }
        }
    }
    x[i]=0; y[i]=0; z[i]=0; u[i]=0; v[i]=0; w[i]=0; color[i]=-1;++i;
    x[i]=0; y[i]=0; z[i]=0; u[i]=0; v[i]=0; w[i]=0; color[i]=1;++i;
    return StaticArray<Array<double>, 7>{x, y, z, u, v, w, color};
    
}

bool dat::plot_lattice_slice(Lattice &lattice, int z, std::string filename)
{

    Array<Array<flt>> latticeSlice = lattice_slice(lattice, z);

    auto fig  = plt::figure(true);
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

bool dat::plot_lattice(Lattice &lattice, std::string filename)
{
    StaticArray<Array<double>, 7> arrays = lattice_Arrays(lattice);
    F64 mag = mean(arrays[6]);

    auto fig  = plt::figure(true);
    fig->size(1000, 1000);
    fig->title("Magnetization: " + to_string(mag));
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

bool dat::convert_pngs_to_gif(string gifname, string pngfilePrefix)
{
    double delay = 5;
    string command =
        "convert -delay " + to_string(delay) + " -layers Optimize " + pngfilePrefix + "*.png " + gifname;
    int i = std::system(command.c_str());
    return i;
}




bool dat::test_all_colors()
{
    plt::palette::rdbu();
    plt::palette::pastel2();
    plt::palette::pastel1();
    Array<Array<double>> map(6, Array<double>(6, 0));
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            map[i][j] = (i * 6 + j) / 36.0;
        }
    }
    plt::imagesc(map);
    plt::colormap(plt::palette::summer());
    plt::save("test_summer.png");
    plt::colormap(plt::palette::hot());
    plt::save("test_hot.png");
    plt::colormap(plt::palette::hsv());
    plt::save("test_hsv.png");
    plt::colormap(plt::palette::parula());
    plt::save("test_parula.png");
    plt::colormap(plt::palette::jet());
    plt::save("test_jet.png");
    plt::colormap(plt::palette::lines());
    plt::save("test_lines.png");
    plt::colormap(plt::palette::colorcube());
    plt::save("test_colorcube.png");
    plt::colormap(plt::palette::prism());
    plt::save("test_prism.png");
    plt::colormap(plt::palette::cool());
    plt::save("test_cool.png");
    plt::colormap(plt::palette::spring());
    plt::save("test_spring.png");
    plt::colormap(plt::palette::autumn());
    plt::save("test_autumn.png");
    plt::colormap(plt::palette::winter());
    plt::save("test_winter.png");
    plt::colormap(plt::palette::bone());
    plt::save("test_bone.png");
    plt::colormap(plt::palette::copper());
    plt::save("test_copper.png");
    plt::colormap(plt::palette::pink());
    plt::save("test_pink.png");
    plt::colormap(plt::palette::gray());
    plt::save("test_gray.png");
    plt::colormap(plt::palette::rdpu());
    plt::save("test_yarg.png");
    plt::colormap(plt::palette::flag());
    plt::save("test_flag.png");
    plt::colormap(plt::palette::prism());
    plt::save("test_prism.png");
    plt::colormap(plt::palette::jet());
    plt::save("test_jet.png");
    plt::colormap(plt::palette::lines());
    plt::save("test_lines.png");
    return true;
}

string dat::get_filename(string Prefix, uint L, F64 J, F64 T, F64 Time)
{
    string filename = Prefix + "L=" + to_string(L) + "_J=" 
                    + to_string(J) + "_T=" + to_string(T) + "K_Time=" 
                    + to_string(Time) + "s.png";
    return filename;
}