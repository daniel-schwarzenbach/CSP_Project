#include <Ploting/PlotLattice.h++>
#include <matplot/matplot.h>
#include <tuple>

namespace plt = matplot;
using std::to_string;

Array<Array<flt>> lattice_slice(Lattice &lattice, uint z = 0)
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

StaticArray<Array<double>, 7> lattice_Arrays(Lattice &lattice)
{
    // get lattice size
    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();
    uint sizeL = Lx * Ly * Lz + 2;
    // initialize Arrays
    Array<double> x(sizeL, 0), y(sizeL, 0), z(sizeL, 0),
        u(sizeL, 0), v(sizeL, 0), w(sizeL, 0),
        color(sizeL, 0);
    uint i = 0;
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

                color[i] = s.phi();
                i++;
            }
        }
    }
    x[sizeL-2]=0;y[sizeL-2]=0;z[sizeL-2]=0;u[sizeL-2]=0;v[sizeL-2]=0;
    w[sizeL-2]=0;color[sizeL-2]=0;
    x[sizeL-1]=0;y[sizeL-1]=0;z[sizeL-1]=0;u[sizeL-1]=0;v[sizeL-1]=0;
    w[sizeL-1]=0;color[sizeL-1]=_2pi_;
    return StaticArray<Array<double>, 7>{x, y, z, u, v, w, color};
}

bool plot_lattice_slice(Lattice &lattice, int z, std::string filename)
{

    Array<Array<flt>> latticeSlice = lattice_slice(lattice, z);
    plt::colormap(rainbow_dark);
    plt::caxis({0, _2pi_});
    plt::gca()->color_box_range(0, _2pi_);
    plt::imagesc(latticeSlice);
    plt::caxis({0, _2pi_});
    plt::gca()->color_box_range(0, _2pi_);
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

bool plot_lattice(Lattice &lattice, std::string filename)
{
    StaticArray<Array<double>, 7> vecs = lattice_Arrays(lattice);
    plt::colormap(rainbow_dark);
    plt::caxis({0, _2pi_});
    plt::gca()->color_box_range(0, _2pi_);
    auto plot = plt::quiver3(vecs[0], vecs[1], vecs[2], vecs[3],
                             vecs[4], vecs[5], vecs[6], 0.5);
    plot->line_width(3);
    plot->normalize(true);
    plt::caxis({0, _2pi_});
    plt::gca()->color_box_range(0, _2pi_);

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

bool convert_pngs_to_gif(string gifname, string pngfilePrefix)
{
    double delay = 5;
    string command =
        "convert -delay " + to_string(delay) + " -layers Optimize " + pngfilePrefix + "*.png " + gifname;
    std::system(command.c_str());

    return 0;
}

bool make_folder(string foldername)
{
    string command = "mkdir -p " + foldername;
    std::system(command.c_str());
    return 0;
}

bool test_all_colors()
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