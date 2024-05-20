#include <Data/Plot.h++>
#include <Measure/Observables.h++>
#include <matplot/matplot.h>
#include <tuple>

namespace plt = matplot;
using std::to_string;

// custom rainbow color map
static Array<Array<double>> rainbow_dark{
    {0.0, 0.0, 0.9},
    {0.0, 0.6, 0.6},
    {0.0, 0.9, 0.0},
    {0.6, 0.6, 0.0},
    {0.9, 0.0, 0.0},
    {0.6, 0.0, 0.6},
    {0.0, 0.0, 0.9}};

// custom heat color map
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

Array2D<f32> lattice_slice(Lattice3D<Spin> &lattice, uint z)
{
    // get dims
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    // copy lattice data
    Array2D<f32> latticeSlice(Lx,
                                   Array<f32>(Ly, 0));
    #pragma omp parallel for collapse(2)
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

StaticArray<Array<f64>, 7> lattice_Arrays(Lattice3D<Spin> &lattice)
{
    const Spin z_vec = {0,0,1};
    // get lattice size
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    // get magnetization
    Vector3 mag = measure::get_magnetization(lattice);
    mag * (1.0 / flt(Lx * Ly * Lz));
    flt maxmag = mag.norm();
    u64 sizeL = Lx * Ly * Lz + 2;
    // initialize Arrays
    Array<double> x(sizeL, 0), y(sizeL, 0), z(sizeL, 0),
        u(sizeL, 0), v(sizeL, 0), w(sizeL, 0),
        color(sizeL, 0);
    // iterarate over all lattice sides
    #pragma omp parallel for collapse(3)
    for (uint ix = 0; ix < Lx; ++ix)
    {
        for (uint iy = 0; iy < Ly; ++iy)
        {
            for (uint iz = 0; iz < Lz; ++iz)
            {
                // label number
                u64 i = lattice.get_raw_id(ix,iy,iz);
                x[i] = ix;
                y[i] = iy;
                z[i] = iz;
                Spin s;
                s = lattice(ix, iy, iz);
                u[i] = s.x();
                v[i] = s.y();
                w[i] = s.z();
                // color the spins according to the magnetization
                color[i] = (s | z_vec);
            }
        }
    }
    u64 i = sizeL-2;
    // set dummy data to -maxmag and +maxmag
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
    return StaticArray<Array<double>, 7>{x, y, z, u, v, w, color};
    
}

bool data::plot_lattice_slice(  Lattice3D<Spin> &lattice, int z, 
                                string filename)
{
    // read in the lattice slice
    Array2D<f32> latticeSlice = lattice_slice(lattice, z);
    // enable figure settings
    auto fig = plt::figure(true);
    // set size in pixel
    fig->size(1000, 1000);
    // set rainbow colors
    plt::colormap(rainbow_dark);
    plt::imagesc(latticeSlice);
    // define the color axis
    plt::caxis({0, _2pi_});
    plt::figure()->size(1000, 1000);
    if (filename != "")
    {
        plt::save(filename); // save the plot
    }
    else
    {
        plt::show(); // just show the plot
    }
    return 0;
    // clean up the entire plotting context
    plt::cla();
}

bool data::plot_lattice(Lattice3D<Spin> &lattice, 
                        std::string filename)
{
    // get quiver vectors
    StaticArray<Array<double>, 7> arrays = lattice_Arrays(lattice);
    // calculates magnetization
    Vector3 mag =measure::get_magnetization(lattice);
    // enables figure settings
    auto fig = plt::figure(true);
    // set size in pixels
    fig->size(1000, 1000);
    // pritn the mag. vector in the Title
    // string title = "Magnetization: {" +to_str(mag(0))+ ","
    //         +to_str(mag(0)) +"," +to_str(mag(0))+ "}";
    const Spin z = {0,0,1};
    string title = "M: " + to_str(mag.norm()) + ",  Mz: " + 
                    to_str(mag | z);
    fig->title(title);
    // get dims
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    // define limits
    uint maxL = max(max(Lx,Lz),Ly);
    plt::xrange({-1.0,flt(maxL)});
    plt::yrange({-1.0,flt(maxL)});
    plt::zlim({-1.0,flt(maxL)});
    // set clolr
    plt::colormap(heat);
    plt::colorbar(true);
    plt::gca()->cblabel("Mz");
    // do not change!!! it works somehow
    plt::gca()->cb_position({1.1f, 0.f, 0.03f, 1.f});
    plt::gca()->position({0.135f, 0.1f, 0.7f, 0.85f});
    // set plotdata
    auto plot = plt::quiver3(arrays[0], arrays[1], arrays[2],
                             arrays[3],
                             arrays[4], arrays[5], arrays[6], 0.5);
    // set line width
    plot->line_width(3);
    plot->normalize(true);
    // label dimension
    plt::xlabel("x");
    plt::ylabel("y");
    plt::zlabel("z");
    // save if the filename was given, else just show
    if (filename != "")
    {
        plt::save(filename);
    }
    else
    {
        plt::show();
    }
    // clean up the entire plotting context
    plt::cla();
    return 0;
}

bool data::plot_lattice_small(Lattice3D<Spin> &lattice, 
                        std::string filename)
{
    // get quiver vectors
    StaticArray<Array<double>, 7> arrays = lattice_Arrays(lattice);
    // calculates magnetization
    flt mag =measure::get_magnetization(lattice).norm();
    // get dims
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    // enable figure settings
    auto fig = plt::figure(true);
    // set figure size in pixel
    fig->size(400, 400);
    // set titles
    string title = "Magnetization: " + to_str(mag);
    fig->title(title);
    // define limits
    plt::xrange({-1.0,flt(Lx)});
    plt::yrange({-1.0,flt(Ly)});
    plt::zlim({-1.0,flt(Lz)});
    // set color
    plt::colormap(heat);
    plt::colorbar(true);
    plt::gca()->cblabel("Magnetization");
    // do not change!!! it works somehow
    plt::gca()->cb_position({1.05f, 0.f, 0.03f, 1.f});
    plt::gca()->position({0.08f, 0.1f, 0.7f, 0.85f});
    // set plotdata
    auto plot = plt::quiver3(arrays[0], arrays[1], arrays[2],
                             arrays[3],
                             arrays[4], arrays[5], arrays[6], 0.5);
    // set line_width
    plot->line_width(3);
    plot->normalize(true);
    // label dimensions
    plt::xlabel("x");
    plt::ylabel("y");
    plt::zlabel("z");
    // save if the filename was given, else just show
    if (filename != "")
    {
        plt::save(filename); // save the file
    }
    else
    {
        plt::show(); // just show the plot
    }
    // clean up the entire plotting context
    plt::cla();
    return 0;
}

bool data::convert_pngs_to_gif(string gifname, string pngFolder)
{
    uint delay = 20;
    string command =
        "convert -delay " + to_string(delay) 
                + " -loop 0 "+ pngFolder + "*.png " + gifname;
    int i = std::system(command.c_str());
    return i;
}

