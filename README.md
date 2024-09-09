Prefer using Visual Studio
1. add external dependencies of ws2_32.lib
2. put libmysql.dll in the main exe file path.
3. add include paths of mysql server in the visual studio.
4. remove the check cin block in connection.cpp
5. import bakend.h in your file, example inclusion is shown in the main.cpp

# TODO - backend completed

- [x] option for MySql database or csv
- [x] write to MySql function
- [x] make the sql database.
- [x] Thread pool.
- [x] Thread synchronisation.
- [x] dynamic wait time and timebase.
- [x] align the dataframes from multiple PMUs.

## 20-02-24 {2021eeb1156}
- [x] drop late packages.
- [x] write dynamic wait times function.
## 19-02-24 {2021eeb1156,2021eeb1157,2021eeb1199,2021eeb1197}
- [x] align the dataframes from multiple PMUs.
## 12-02-24 {2021eeb1156,2021eeb1157,2021eeb1199,2021eeb1197}
- [x] accept data from multiple pmu.
- [x] close socket logic implemented.
## 08-02-24 {2021eeb1156,2021eeb1157,2021eeb1199,2021eeb1197}
- [x] write into a live txt.
- [x] vary the data stream according to the stat word. what if stat changes more than once in the minute.
## 05-02-24 {2021eeb1156,2021eeb1157,2021eeb1199,2021eeb1197}
- [x] write in csv.
- [x] write code for 16bit data.
- [x] dynamic file naming.
## 03-02-24 {2021eeb1156}
- [x] make connection.cpp modular.  
- [x] structure the data printing.  
- [x] structure all the files.

