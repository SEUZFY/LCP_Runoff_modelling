#include <iostream>
#include <queue>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include "gdal_priv.h"
#include "cpl_conv.h"

using std::cout;
using std::ios;
using std::cerr;
using std::ofstream;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::this_thread::sleep_for;
using std::future;
using std::launch;
using std::async;


// Storage and access of a raster of a given size
struct Raster {
    std::vector<int> pixels; // where everything is stored
    int nrows, ncols; // number of rows and cols

    // Initialise a raster with rows and cols
    Raster(int& rows, int& cols) {
        nrows = rows;
        ncols = cols;
        unsigned int total_pixels = rows * cols; //unsigned int for big size
        pixels.reserve(total_pixels);
    }

    // Fill values of an entire row
    void add_scanline(const int* line) {
        for (int i = 0; i < ncols; ++i) pixels.push_back(line[i]);
        
    }

    // Fill entire raster with zeros
    void fill() {
        unsigned int total_pixels = nrows * ncols;
        for (unsigned int i = 0; i < total_pixels; ++i) pixels.push_back(0);
    }

    // Access the value of a raster cell to read or write it
    int& operator()(int& row, int& col) {
        assert(row >= 0 && row < nrows);
        assert(col >= 0 && col < ncols);
        return pixels[col + row * ncols];
    }

    // Access the value of a raster cell to read it
    int operator()(int& row, int& col) const {
        assert(row >= 0 && row < nrows);
        assert(col >= 0 && col < ncols);
        return pixels[col + row * ncols];
    }
};

// A structure that links to a single cell in a Raster
struct RasterCell {
    int row, col; // row and column of the cell
    int elevation;
    int insertion_order;

    // Defines a new link to a cell
    RasterCell(int& c_row, int& c_col, int& elevation, int& insertion_order) {
        this->row = c_row;
        this->col = c_col;
        this->elevation = elevation;
        this->insertion_order = insertion_order;
    }

    // Define the order of the linked cells (to be used in a priority_queue)
    bool operator<(const RasterCell& other) const {
        // to do with statements like if (this->elevation > other.elevation) return false/true;
        return (other.elevation) > (this->elevation) ? true : false;
    }
};

// Write the values in a linked raster cell (useful for debugging)
std::ostream& operator<<(std::ostream& os, const RasterCell& c) {
    os << "{h=" << c.elevation << ", o=" << c.insertion_order << ", row=" << c.row 
        << ", col=" << c.col << "}";
    return os;
}

// Write the raster file to an .asc file
void output_raster(const Raster& raster, const double& pixelsize, const double& topx, const double& topy)
{
    int ncols(raster.ncols), nrows(raster.nrows);
    double lowy(topy - pixelsize * raster.nrows);

    ofstream outfile("D:/AlbertQ2/GEO1015/test.asc", ios::out); 
    if (!outfile)cout << "File open issue, please check. " << '\n';
    else
    {
        outfile << "NCOLS" << " " << ncols << '\n';
        outfile << "NROWS" << " " << nrows << '\n';
        outfile << "XLLCORNER" << " " << topx << '\n'; // low-left and up-left: x coordinate is same
        outfile << "YLLCORNER" << " " << lowy << '\n'; // origin y is topy, need to be converted to lowy
        outfile << "CELLSIZE" << " " << pixelsize << '\n';
        outfile << "NODATA_VALUE" << " " << -9999 << '\n';

        for (int i = 0; i < nrows; ++i)
        {
            for (int j = 0; j < ncols; ++j) outfile << raster(i, j) << " ";
            outfile << '\n';
        }
        outfile.close(); // close the file
    }  
}


Raster flow_direction(Raster raster)
{
    sleep_for(seconds(2));
    return raster;
}


Raster flow_accumulation(Raster raster)
{
    sleep_for(seconds(2));
    return raster;
}

int test()
{
    sleep_for(seconds(2));
    return 0;
}


int main(int argc, const char* argv[])
{
    
    //Raster r(3, 3);
    //r.fill();
    //r(2, 2) = 1;
    //std::cout<<r(0,0);

    //RasterCell cella(0, 0, 20, 1), cellb(0, 0, 30, 2);
    //std::cout << (cella < cellb);

    ios::sync_with_stdio(false); //speed up for cin and cout

    GDALDataset* input_dataset = nullptr;
    GDALAllRegister();
    input_dataset = (GDALDataset*)GDALOpen("D:/AlbertQ2/GEO1015/N25W101.hgt", GA_ReadOnly); // a nice tile for testing
    if (input_dataset == nullptr) {
        cerr << "Couldn't open file" << '\n';
        return 1;
    }

    //Print dataset info
    cout << '\n';
    cout << "dataset info: " << '\n';

    double geo_transform[6]{ 0 };
    cout << "Driver: " << input_dataset->GetDriver()->GetDescription()
        << "/" << input_dataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME) << '\n';
    cout << "width size is: " << input_dataset->GetRasterXSize() << '\n';
    cout << "hight size is: " << input_dataset->GetRasterYSize() << '\n';
    cout << "the number of raster bands: " << input_dataset->GetRasterCount() << '\n';
    
    if (input_dataset->GetProjectionRef() != NULL) cout << "Projection is '" << input_dataset->GetProjectionRef() << "'" << '\n';
    if (input_dataset->GetGeoTransform(geo_transform) == CE_None) {
        cout << "Origin = (" << geo_transform[0] << ", " << geo_transform[3] << ")" << '\n';
        cout << "Pixel Size = (" << geo_transform[1] << ", " << geo_transform[5] << ")" << '\n';
    }//Origin is the top-left corner

    // Print Band 1 info
    cout << '\n';
    cout << "Band 1 info: " << '\n';

    GDALRasterBand* input_band = nullptr;
    int nBlockXSize, nBlockYSize;
    int bGotMin, bGotMax;
    double adfMinMax[2]{ 0 };
    input_band = input_dataset->GetRasterBand(1);
    input_band->GetBlockSize(&nBlockXSize, &nBlockYSize); //XSize=3601, YSize=1, "scanline"
    cout << "Band 1 Block=" << nBlockXSize << " x " << nBlockYSize << " Type=" 
        << GDALGetDataTypeName(input_band->GetRasterDataType()) << " ColorInterp=" 
        << GDALGetColorInterpretationName(input_band->GetColorInterpretation()) << '\n';

    adfMinMax[0] = input_band->GetMinimum(&bGotMin);
    adfMinMax[1] = input_band->GetMaximum(&bGotMax);
    if (!(bGotMin && bGotMax)) GDALComputeRasterMinMax((GDALRasterBandH)input_band, TRUE, adfMinMax);
    cout << "Min=" << adfMinMax[0] << " Max=" << adfMinMax[1] << '\n';

    // Read Band 1 line by line
    int nXSize(input_band->GetXSize()); //width(ncols)
    int nYSize(input_band->GetYSize()); //height(nrows)
    Raster input_raster(nYSize, nXSize); //Raster(nrows, ncols)

    int* scanline((int*)CPLMalloc(sizeof(float) * nXSize));
    for (int current_scanline = 0; current_scanline < nYSize; ++current_scanline)
    {
        //int* scanline((int*)CPLMalloc(sizeof(float) * nXSize)); // DONT forget to use CPLFree(scanline)
        if (input_band->RasterIO(GF_Read, 0, current_scanline, nXSize, 1,
            scanline, nXSize, 1, GDT_Int32,
            0, 0) != CPLE_None)
        {
            cerr << "Couldn't read scanline " << current_scanline << '\n';
            return 1;
        }
        input_raster.add_scanline(scanline);
        //CPLFree(scanline); //corresponding to the allocation "scanline"
    }
    CPLFree(scanline);

    cout << "Created raster: " << input_raster.nrows << " x " 
        << input_raster.ncols << " = " << input_raster.pixels.size() << '\n';
    
    int Sum(0);
    // Get Start Time
    system_clock::time_point start = system_clock::now();

    //flow_direction(input_raster);
    //future<Raster> resultFromDirection = async(launch::async, flow_direction, input_raster);
    future<int> resultFromDirection = async(launch::async, test);
    flow_accumulation(input_raster);
    resultFromDirection.get();

    // Get End Time
    auto end = system_clock::now();
    auto diff = duration_cast <seconds> (end - start).count();
    cout << "Total Time Taken = " << diff << " Seconds" << '\n';


    //output_raster(input_raster, geo_transform[1], geo_transform[0], geo_transform[3]);
    
    // Flow direction
    //Raster flow_direction(input_raster.ncols, input_raster.nrows);
    //flow_direction.fill();
    //std::priority_queue<RasterCell, std::deque<RasterCell>> cells_to_process_flow;
    // to do

    // Write flow direction
    // to do

    // Flow accumulation
    //Raster flow_accumulation(input_raster.ncols, input_raster.nrows);
    // to do

    // Write flow accumulation
    // to do

    // Close input dataset
    GDALClose(input_dataset);

	return 0;
}