# The program is to read pressure values from the pressure csv file and store the time and corresponding pressure values into an output pressurecsv.h
# For 2016 ; We use USC pressure values
# For 2017 :  We should use USC pressure values
# For 2018 : We should use MiliQan pressure values with better least count

#Run this prgram over the pressure file with only the above pressure sensor values
#Modify the program for each year
import csv
import os
from datetime import datetime
debug = False
def split_csv(path_csv, path_run_list, year):
    f = open(path_csv)
    print("file opening" , path_csv)
    output_string = path_run_list
    output_file = open(output_string,"w") 
    lines = f.readlines()[2:]
    first_line = "double getpressure"+year+"(UInt_t time){\n"
    output_file.write(first_line)
   # output_file.write("run\tstart_time\tend_time\n") 
    for line in lines:
            #print(line)
            line = line.strip()
            splitted_line = line.split(",")
            time_value = splitted_line[0]
            pressure_value = splitted_line[1]
            #print("time value ",time_value) 
            time_value = time_value.replace('"','')
            time_value = int(time_value) 
            pressure_value = pressure_value.replace('"','')
            pressure_value = round(float(pressure_value),3)


            if(pressure_value ==-100) :
                 print("time : pressure : ",time_value, " : ",pressure_value)
                 continue
            #print(time_value, " ", pressure_value)            
            #print("time ", int(time_value))
            #print( " pressure ",float(pressure_value))
            new_string = "else if(time < "+str(time_value)+") return "+str(pressure_value)+";\n"
            output_file.write(new_string)
   
    output_file.write("else return 972;\n")
    output_file.write("}")
 
if __name__ == '__main__':
    year = "2017"
    path_csv = f"./input_pressure_{year}.csv"
    path_run_list = f"./pressurecsc_{year}.h"
    split_csv(path_csv,path_run_list, year)

