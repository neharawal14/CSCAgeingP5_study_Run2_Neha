import re
import csv
def split_csv(path):
    f = open(path)
    my_file = open("./IntegrateLumi_2016.h","w")
    #my_file.write("date\tIntegrated_lumi")
    #my_file.write("\n")
    data = []
    count = 0
    previous_date = "string"
    delievered_lumi=0
    integrated_lumi=0
    full_date_list=[]
    integrated_lumi_list = []
    for line in f:
        if(line[0]=="#"):
            continue
        new_line=line.split(",")
       # new_line=new_line1.split(":")
        count = count + 1
        #if(count>3):
        #    if (line[2:4] == '--'):
        #        print("happening once ")
        #        break
          #  if "x 12:48:14 " in line[2]:
             #   print(" the x event")
          #      break;
        run_nb_fill = new_line[0]
        print("run nb", run_nb_fill)
        print("line ", new_line)
        run_nb_list = run_nb_fill.split(":")
        run_nb = run_nb_list[0]
        date_time = new_line[1]
        print("run_nb ,date time here ",run_nb, "  ",date_time," delievered ", float(new_line[4]))
        delievered_lumi = delievered_lumi+float(new_line[4])
        print(delievered_lumi)
        new_delievered_lumi = delievered_lumi
        print(new_delievered_lumi)
##        run_new= run_nb.split(":")
        date_list= date_time.split(" ")
        date = date_list[0]
        time = date_list[1]
        print(date, " time ", time)
        full_date_list.append(date)
#
#            #print("integrated_lumi in every new date ", integrated_lumi)
#            #if date!= previous_date:
#             #   print("before listing in list ", date)
#              #  print("integrated_lumi calculated in loop ",integrated_lumi)
#
#
        integrated_lumi_list.append(new_delievered_lumi)
#
       #test_string="if(time==" +date[0]+")return "+str(new_recorded_lumi)+";"
       #print(test_string)
#       my_file.write(date)
        line_string = "if(run == "+str(run_nb)+") return "+ str(new_delievered_lumi) + ";"  
        my_file.write(line_string) 
#        my_file.write("\t")
#       my_file.write(str(new_delievered_lumi))
        my_file.write("\n")


    print(full_date_list)
    print(integrated_lumi_list)
    print("number of elements in date list",len(full_date_list))
    print("number of elements in integrated lumi list", len(integrated_lumi_list))
    my_file.close()

if __name__ == '__main__':
    path_csv = "./luminoisty_2016.csv"
    split_csv(path_csv)

