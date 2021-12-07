#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include <future>
#include "gdal_priv.h"
#include "cpl_conv.h"

#include "Raster.h"
#include "Direction.h"

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


// Write the values in a linked raster cell (useful for debugging)
std::ostream& operator<<(std::ostream& os, const RasterCell& c) {
    os << "{h=" << c.elevation << ", o=" << c.insertion_order << ", row=" << c.row 
        << ", col=" << c.col << "}";
    return os;
}


void flow_direction(Raster* raster)
{
    sleep_for(seconds(2));
}


void flow_accumulation(Raster raster)
{
    sleep_for(seconds(2));
}


int main(int argc, const char* argv[])
{  
    
    ios::sync_with_stdio(false); //speed up for cin and cout

    GDALAllRegister();
    GDALDataset* input_dataset((GDALDataset*)GDALOpen("D:/AlbertQ2/GEO1015/N52E020.hgt", GA_ReadOnly));
    if (!input_dataset) {
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

    GDALRasterBand* input_band(input_dataset->GetRasterBand(1));
    int nBlockXSize, nBlockYSize;
    int bGotMin, bGotMax;
    double adfMinMax[2]{ 0 };
    
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
    for (int current_scanline = 0; current_scanline != nYSize; ++current_scanline)
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
    

    //minimum heap
    std::priority_queue<RasterCell, std::deque<RasterCell>> cells_queue;

    /*
    Raster input_raster(3, 3), d1(3, 6);
    input_raster.fill();
    input_raster(0, 0) = 9; input_raster(0, 1) = 8; input_raster(0, 2) = 7;
    input_raster(1, 0) = 8; input_raster(1, 1) = 7; input_raster(1, 2) = 6; 
    input_raster(2, 0) = 7; input_raster(2, 1) = 6; input_raster(2, 2) = 5; */

    // insert: global variable, standing for the insertion order
    int insert(0);

    ProRaster flow_direction(input_raster.nrows, input_raster.ncols);
    flow_direction.fill_proraster(input_raster);

    //add the potential outlets: boundary, adding order: clockwise
    add_outlets_boundary(input_raster.nrows, input_raster.ncols, flow_direction, cells_queue,insert);

    /*
    for (int i = 0; i < flow_direction.nrows; ++i)
    {
        for (int j = 0; j < flow_direction.ncols; ++j)
            cout << flow_direction(i, j).direction << " ";
        cout << '\n';
    }
    */

    //vector to store the cells: opposite order of the cells_queue
    std::vector<RasterCell> cells_vector;

    //compute flow direction
    compute_flow_direction(flow_direction, cells_queue, cells_vector, insert);

    //compute flow accumulation   
    compute_flow_accumulation(flow_direction, cells_vector);
   
    output_raster(flow_direction, geo_transform[1], geo_transform[0], geo_transform[3]);

    /*
    for (int i = 0; i < flow_direction.nrows; ++i)
    {
        for (int j = 0; j < flow_direction.ncols; ++j)
            cout << flow_direction(i, j).direction << " ";
        cout << '\n';
    }*/

    //cout << "\n";

    //add_neighbours(1, 1, flow_direction, cells_queue, insert);

    //identify the adjacent pixel types
    /*for (int i = 0; i < d1.nrows; ++i)
    {
        for (int j = 0; j < d1.ncols; ++j)
            cout << adjacent_pixel_types(i, j, d1) << " ";
        cout << '\n';
    }
    cout << '\n';
    cout << adjacent_pixel_types(6, 6, d1);*/
    
    
    //cout << insert << '\n';

   
    

    //Raster r(3, 3);
    //r.fill();
    //r(2, 2) = 1;
    //std::cout<<r(2,2);

    //RasterCell cella(0, 0, 20, 1), cellb(0, 0, 30, 2);
    //std::cout << (cella < cellb);
    
    // Get Start Time
    //system_clock::time_point start = system_clock::now();

    //Raster r_flow_direction(input_raster.nrows, input_raster.ncols);
    //r_flow_direction.fill();

    //future<void> resultFromDirection = async(launch::async, flow_direction, &r_flow_direction);
    //flow_accumulation(input_raster);
    //resultFromDirection.get();

    //// Get End Time
    //auto end = system_clock::now();
    //auto diff = duration_cast <seconds> (end - start).count();
    //cout << "Total Time Taken = " << diff << " Seconds" << '\n';


    //output_raster(input_raster, geo_transform[1], geo_transform[0], geo_transform[3]);
   

    // Flow direction
    //Raster flow_direction(input_raster.nrows, input_raster.ncols);
    //flow_direction.fill();
    
    //std::priority_queue<RasterCell, std::deque<RasterCell>> cells_queue;
    //unsigned int size(input_raster.pixels.size());
    //int order(0);
    //for (int i = 0; i != nYSize; ++i)
    //{
    //    for (int j = 0; j != nXSize; ++j)
    //    {
    //        //calculate insertion order for each cell: order
    //        order += 1;
    //        cells_queue.push(RasterCell(i, j, input_raster(i, j), order));
    //    }         
    //}

    //while (!cells_queue.empty())
    //{
    //     cout << cells_queue.top() << ' ';
    //     cells_queue.pop();
    //} 


    // to do

    // Write flow direction
    // to do

    // Flow accumulation
    //Raster flow_accumulation(input_raster.nrows, input_raster.ncols);
    // to do

    // Write flow accumulation
    // to do

    // Close input dataset
    GDALClose(input_dataset);

	return 0;
}