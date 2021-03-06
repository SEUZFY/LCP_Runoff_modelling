# LCP implementation for flow direction and flow accumulation.
The idea is from this paper: https://hess.copernicus.org/articles/15/667/2011/

 * The source code is inspired by the skeleton:
 * https://gitlab.tudelft.nl/3d/geo1015.2021/ Original author: Ken Arroyo Ohori (https://3d.bk.tudelft.nl/ken/en/)
 * If a bug is found or there is a better way to implement it, please do not hesitate to contact:
 * F.Zhang-12@student.tudelft.nl(or zmocheng@gmail.com)


**-- REMINDER**

This project was compiled in Visual Studio 2019 Community on 64 bit windows platform, that being said, it may not directly work on a mac/linux platform, though a cmake project can be built with all source files in: assignment2 -> src. (**External dependency**: **GDAL**).
 
 
**-- HOW TO USE**

An package can be downloaded at: https://github.com/SEUZFY/LCP_Runoff_modelling/releases

Or just clone this repository in a x64 windows computer, then compile and run it in Visual Studio(version >= 2019).

Enter into:

**assignment2 -> x64 -> Release**, and you’ll be able to see **assignment2.exe** file, open the command window here and pass the **FULLPATH** of the input file as an argument for the program(**NB**: this **FULLPATH** is also where the **generated files** will go):

**...\assignment2\x64\Release>assignment2.exe FULLPATH**

An example:

![image](https://user-images.githubusercontent.com/72781910/146770183-5cd9ccc2-eccc-4f5a-b48f-3000628f499c.png)

**NB**: If no argument is supplied, the program won’t execute the process of flow direction and flow accumulation, instead it will give an prompt to require an argument to execute the expected process.

**-- INPUT EXAMPLE**

An hgt file: download from this website: http://dwtkns.com/srtm30m/

![image](https://user-images.githubusercontent.com/72781910/146772130-df7528a2-9637-4bd6-b926-9219a9a31291.png)

**-- OUTPUT EXAMPLE**

**- flow direction**

![image](https://user-images.githubusercontent.com/72781910/146772355-571680fa-6981-4e70-a6b1-9205fdaae3f1.png)

**- flow accumulation**

![image](https://user-images.githubusercontent.com/72781910/146772454-b8311605-03d1-4c1e-8697-2dc8a027c599.png)

**NB**: If you open the accumulation.tif in QGIS, you may adjustment the min/max values in the properties setting in order to obtain a more detailed display(**Properties -> Min/Max Values Settings -> Select "Cumulative count cut"**).

![image](https://user-images.githubusercontent.com/72781910/146772890-310fee57-a6f8-4504-9486-cdfac590c274.png)

**-- GOOD TO KNOW**

If there is a large area of sea or inland lake in the input file, it may affect the results. Although the LCP algorithm will not be greatly affected by that, the input file without a large area of sea or inland lake may lead to more pleasant results.

**-- SPECIAL THANKS**

Thanks for my collaborator Yitong Xia(xiayitong0630@gmail.com). During the implementation of this algorithm, we sincerely discussed and compared each other's execution results, thank her for her efforts.

And particular thanks go to my good friend Lin(linyinjia@outlook.com). I discussed with him whether to use _emplace_back()_ instead of _push_back()_ in the process of using _std::vector_. And we discussed on the topic of execution efficiency of _static_ array and _std::vecotr_ array.
