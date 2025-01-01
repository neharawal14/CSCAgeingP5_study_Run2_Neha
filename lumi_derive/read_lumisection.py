import csv

def split_csv(path):
    f = open(path)
    data = []
    count = 0
    for line in f:
        if(line[0]=="#"):
            continue
        run_nb = line[:6]
        data.append(run_nb)
        first="./instlumi/"+data[0] +".csv"
        tmp=0
        if ((data[0] == data[count])):
            if(count==0):
                new_file1 = open(first,"w")
                print(first)
            new_file1.write(line)
        else:
            if(data[count-1] != data[count]):
                    filename_string="./instlumi/"+line[:6]+".csv"
                    new_file=open(filename_string,"w")
                #print(filename_string)
            new_file.write(line)
        count = count + 1

if __name__ == '__main__':
    path_csv = "my2016lumibyls.csv"
    split_csv(path_csv)

