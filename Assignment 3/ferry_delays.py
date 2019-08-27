# Andrew Yang
# SENG 265
# Daniela Damian
# V00878595
# 11/18/17

import sys
import csv

# list of month abbreviations
month_list = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']


def main():
    # prompt for port selection
    port_select = port_prompt()
    if(port_select == "q"):
            return

    # prompt for month selection
    month_select = month_prompt()
    if(month_select == "q"):
            return

    # runs a loop unless broken by "q"
    while(True):
        # List of eligible files
        file_data = []
        for x in range(1, len(sys.argv)):
            # opens up the file to check if its eligible
            tmp_dict = csv_reader(str(sys.argv[x]))
            if int(tmp_dict[2]['scheduled_departure_month']) == month_select:
                file_data.append(sys.argv[x])

        # calculates the average based on port_select
        if port_select.lower() == 't':
            Tsawwassen_avg(file_data, month_select)
        else:
            Swartz_avg(file_data, month_select)

        # reprompts the user for a new average
        port_select = port_prompt()
        if(port_select == "q"):
            break

        month_select = month_prompt()
        if(month_select == "q"):
            break


# this handles prompting the user for the port selection
def port_prompt():
    prompt_port = "Which port would you like delay statistics on? Enter the letter: Tsawwassen (t) or Swartz Bay (s) "
    port_select = input(prompt_port)
    prompt_invport = "Please enter a valid port: Tsawwassen (t) or Swartz Bay (s) "

    valid_port = 0
    while valid_port == 0:
        if(port_select.lower() == 't'):
            valid_port = 1
        elif(port_select.lower() == 's'):
            valid_port = 1
        elif(port_select.lower() == 'q'):
            valid_port = 1
        else:
            port_select = input(prompt_invport)

    return port_select


# this handles prompting the user for the month selection
def month_prompt():
    prompt_month = ("Which month would you like average delay time from? Please choose a number from 1 - 12 (January (1) - December (12)) ")
    month_select = input(prompt_month)
    prompt_invmonth = ("Please enter a number from 1 - 12: ")

    valid_month = 0
    while valid_month == 0:
        if(str(month_select) != 'q'):
            try:
                month_select = int(month_select)
                valid_month = 1
            except ValueError:
                month_select = input(prompt_invmonth)
            if valid_month == 1:
                if 0 < int(month_select) < 13:
                    valid_month = 1
                else:
                    valid_month = 0
                    month_select = input(prompt_invmonth)

    return month_select


# reads the csv file and puts it into a dictionary
def csv_reader(input_file):
    with open(input_file, mode='r') as infile:
        list_csv = [{k: v for k,v in row.items()} 
            for row in csv.DictReader(infile, skipinitialspace=True)]
    return list_csv


# calculates the difference in time
def get_time(time_dict):
    sch_day = time_dict['scheduled_departure_day']
    act_day = time_dict['actual_departure_day']

    sch_hour = time_dict['scheduled_departure_hour']
    act_hour = time_dict['actual_departure_hour']

    sch_min = time_dict['scheduled_departure_minute']
    act_min = time_dict['actual_departure_minute']

    if sch_day == act_day:
        time = int(act_hour) - int(sch_hour)
        time = time * 60
        time += (int(act_min) - int(sch_min))
        return time
    else:
        return 0


# calculates the average based on our dictionaries
def Swartz_avg(month_files, user_month):
    average = 0
    count = 0
    total_time = 0

    for months in month_files:
        Swar_dict = csv_reader(months)
        for x in range(0, len(Swar_dict)):
            if Swar_dict[x]['departure_terminal'] == 'Swartz Bay':
                total_time += get_time(Swar_dict[x])
                count += 1

    average = total_time / count
    print("RESULTS")
    print("Tsawwassen:")
    print("    Average delay for " + month_list[user_month - 1] + ": " + "{0:.2f}".format(average))
    print("END")


# calculates the average based on our dictionaries
def Tsawwassen_avg(month_files, user_month):
    average = 0
    count = 0
    total_time = 0

    for months in month_files:
        Tsaw_dict = csv_reader(months)    
        for x in range(0, len(Tsaw_dict)):
                if Tsaw_dict[x]['departure_terminal'] == 'Tsawwassen':
                    total_time += get_time(Tsaw_dict[x])
                    count += 1

    average = total_time / count
    print("RESULTS")
    print("Tsawwassen:")
    print("    Average delay for " + month_list[user_month - 1] + ": " + "{0:.2f}".format(average))
    print("END")


if __name__ == '__main__':
    main()
