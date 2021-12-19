/*********************************************************
 * The source code is inspired by the skeleton:
 * https://gitlab.tudelft.nl/3d/geo1015.2021/
 *
 * Original author: 
 * Ken Arroyo Ohori
 * https://3d.bk.tudelft.nl/ken/en/
 *
 *********************************************************
 * 
 * The whole project and related files are available at:
 * https://github.com/SEUZFY/assignment2
 * If a bug is found or there is a better way to implement it, please do not hesitate to contact:
 * F.Zhang-12@student.tudelft.nl(or zmocheng@gmail.com)
 * 
 *********************************************************/


#include <iostream>
#include "cpl_conv.h"
#include "string"
#include "DA.h"


// #include <queue>
// #include <chrono>
// #include <thread>
// #include <future>
// #include "gdal_priv.h"
// #include "Raster.h"


// using std::chrono::system_clock;
// using std::chrono::duration_cast;
// using std::chrono::seconds;
// using std::this_thread::sleep_for;
// using std::future;
// using std::launch;
// using std::async;


//  Write the values in a linked raster cell (useful for debugging)
std::ostream& operator<<(std::ostream& os, const RasterCell& c) {
    os << "{h=" << c.elevation << ", o=" << c.insertion_order << ", row=" << c.row 
        << ", col=" << c.col << "}";
    return os;
}


/*
void flow_direction(Raster* raster)
{
    sleep_for(seconds(2));
}*/

/*
void flow_accumulation(Raster raster)
{
    sleep_for(seconds(2));
}*/


int main(int argc, const char* argv[])
{  
    
    std::ios::sync_with_stdio(false); // speed up for std::cin and std::cout

    if (argc <= 1) {
        std::cout << "This is: " << argv[0] << '\n';
        std::cout << '\n';
        std::cout << "Please call this program with one input file name(full path) as a program argument" << '\n';
        std::cout << '\n';

        return EXIT_FAILURE;
    }
    else {
        GDALAllRegister();
        const char* srcfile = argv[1]; // file name of the input dataset
        GDALDataset* input_dataset((GDALDataset*)GDALOpen(srcfile, GA_ReadOnly));
        if (!input_dataset) {
            std::cerr << "Couldn't open file" << '\n';
            return 1;
        }

        // Print dataset info
        std::cout << '\n';
        std::cout << "dataset info: " << '\n';

        double geo_transform[6]{ 0 };
        std::cout << "Driver: " << input_dataset->GetDriver()->GetDescription()
            << "/" << input_dataset->GetDriver()->GetMetadataItem(GDAL_DMD_LONGNAME) << '\n';
        std::cout << "width size is: " << input_dataset->GetRasterXSize() << '\n';
        std::cout << "hight size is: " << input_dataset->GetRasterYSize() << '\n';
        std::cout << "the number of raster bands: " << input_dataset->GetRasterCount() << '\n';

        if (input_dataset->GetProjectionRef() != NULL) std::cout << "Projection is '" << input_dataset->GetProjectionRef() << "'" << '\n';
        if (input_dataset->GetGeoTransform(geo_transform) == CE_None) {
            std::cout << "Origin = (" << geo_transform[0] << ", " << geo_transform[3] << ")" << '\n';
            std::cout << "Pixel Size = (" << geo_transform[1] << ", " << geo_transform[5] << ")" << '\n';
        }// Origin is the top-left corner

        // Print Band 1 info
        std::cout << '\n';
        std::cout << "Band 1 info: " << '\n';

        GDALRasterBand* input_band(input_dataset->GetRasterBand(1));
        int nBlockXSize, nBlockYSize;
        int bGotMin, bGotMax;
        double adfMinMax[2]{ 0 };

        input_band->GetBlockSize(&nBlockXSize, &nBlockYSize); // XSize=3601, YSize=1, "scanline"
        std::cout << "Band 1 Block=" << nBlockXSize << " x " << nBlockYSize << " Type="
            << GDALGetDataTypeName(input_band->GetRasterDataType()) << " ColorInterp="
            << GDALGetColorInterpretationName(input_band->GetColorInterpretation()) << '\n';

        adfMinMax[0] = input_band->GetMinimum(&bGotMin);
        adfMinMax[1] = input_band->GetMaximum(&bGotMax);
        if (!(bGotMin && bGotMax)) GDALComputeRasterMinMax((GDALRasterBandH)input_band, TRUE, adfMinMax);
        std::cout << "Min=" << adfMinMax[0] << " Max=" << adfMinMax[1] << '\n';

        // Read Band 1 line by line
        int nXSize(input_band->GetXSize()); // width(ncols)
        int nYSize(input_band->GetYSize()); // height(nrows)
        ProRaster input_raster(nYSize, nXSize); // Raster(nrows, ncols)

        int* scanline((int*)CPLMalloc(sizeof(float) * nXSize));
        for (int current_scanline = 0; current_scanline != nYSize; ++current_scanline)
        {
            // int* scanline((int*)CPLMalloc(sizeof(float) * nXSize)); // DONT forget to use CPLFree(scanline)
            if (input_band->RasterIO(GF_Read, 0, current_scanline, nXSize, 1,
                scanline, nXSize, 1, GDT_Int32,
                0, 0) != CPLE_None)
            {
                std::cerr << "Couldn't read scanline " << current_scanline << '\n';
                return 1;
            }
            input_raster.add_scanline(current_scanline, scanline);
            // CPLFree(scanline); //corresponding to the allocation "scanline"
        }
        CPLFree(scanline);

        std::cout << "Created raster: " << input_raster.nrows << " x "
            << input_raster.ncols << " = " << input_raster.propixels.size() << '\n';

        // minimum heap
        std::priority_queue<RasterCell, std::deque<RasterCell>> cells_queue;

        // insert: global variable, standing for the insertion order
        int insert(0);

        // add the potential outlets : boundary, adding order : clockwise
        add_outlets_boundary(input_raster.nrows, input_raster.ncols, input_raster, cells_queue, insert);

        // vector to store the cells : opposite order of the cells_queue
        std::vector<RasterCell> cells_vector;

        compute_flow_direction(input_raster, cells_queue, cells_vector, insert);

        compute_flow_accumulation(input_raster, cells_vector);

        // output the result rasters, named by the input dataset
        
        // output flow direction file

        std::string extension("flow_direction.tif"), tiffname(argv[1]);
        tiffname += extension; // set the name and path of the flow_direction file
        GDALDriver* driverGeotiff(GetGDALDriverManager()->GetDriverByName("GTiff"));
        if (!driverGeotiff) {
            std::cerr << "Couldn't set driver" << '\n';
            return 1;
        }
        GDALDataset* geotiffDataset(driverGeotiff->Create(tiffname.c_str(), nXSize, nYSize, 1, GDT_Int32, NULL));
        if (!geotiffDataset) {
            std::cerr << "Couldn't set geotiff file" << '\n';
            return 1;
        }
        geotiffDataset->SetGeoTransform(geo_transform);
        geotiffDataset->SetProjection(input_dataset->GetProjectionRef());

        unsigned int* output_line((unsigned int*)CPLMalloc(sizeof(unsigned int)* nXSize));

        for (int current_scanline = 0; current_scanline != nYSize; ++current_scanline)
        {
            input_raster.output_direction(current_scanline, output_line); // first assign the values to line[i]
            
            if (geotiffDataset->GetRasterBand(1)->RasterIO(GF_Write, 0, current_scanline, nXSize, 1,
                output_line, nXSize, 1, GDT_UInt32, 0, 0) != CPLE_None) // write the values into the raster file
            {
                std::cerr << "Couldn't load output_line " << current_scanline << '\n';
                return 1;
            }
                      
        }

        // output the flow accumulation file

        extension = "flow_accumulation.tif";
        tiffname = argv[1];
        tiffname += extension; // set the name and path of the flow_direction file
        geotiffDataset = driverGeotiff->Create(tiffname.c_str(), nXSize, nYSize, 1, GDT_Int32, NULL);
        if (!geotiffDataset) {
            std::cerr << "Couldn't set geotiff file" << '\n';
            return 1;
        }
        geotiffDataset->SetGeoTransform(geo_transform);
        geotiffDataset->SetProjection(input_dataset->GetProjectionRef());

        for (int current_scanline = 0; current_scanline != nYSize; ++current_scanline)
        {
            input_raster.output_accumulation(current_scanline, output_line); // first assign the values to line[i]
            
            if (geotiffDataset->GetRasterBand(1)->RasterIO(GF_Write, 0, current_scanline, nXSize, 1,
                output_line, nXSize, 1, GDT_UInt32, 0, 0) != CPLE_None) // write the values into the raster file
            {
                std::cerr << "Couldn't load output_line " << current_scanline << '\n';
                return 1;
            }

        }

        CPLFree(output_line);

        // Close
        GDALClose(geotiffDataset);
        GDALClose(input_dataset);
        GDALDestroyDriverManager();

        std::cout << "done" << '\n';
        std::cout << "You should be able to have the two geotiff files in the same folder directory of the input dataset" << '\n';

        return EXIT_SUCCESS; // EXIT_SUCCESS

    }

}

    /*
    // Get Start Time
    system_clock::time_point start = system_clock::now();

    Raster r_flow_direction(input_raster.nrows, input_raster.ncols);
    r_flow_direction.fill();

    future<void> resultFromDirection = async(launch::async, flow_direction, &r_flow_direction);
    flow_accumulation(input_raster);
    resultFromDirection.get();

    // Get End Time
    auto end = system_clock::now();
    auto diff = duration_cast <seconds> (end - start).count();
    std::cout << "Total Time Taken = " << diff << " Seconds" << '\n';*/


   

   
