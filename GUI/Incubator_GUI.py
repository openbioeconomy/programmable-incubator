# GUI for DNA Incubator #
# Created by Stephen Peters April 2025 #
# MIT Licence #

### VSCODE INSTRUCTIONS ###
# python --version Python 3.11.6
# python -m venv venv
# selected the venv python interpreter
# pip install dearpygui
# python.exe -m pip install --upgrade pip

### V 0.1 Notes ###
# plenty of work to be done before this is actually useable
# this version gives an idea of layout and basic functionalities

### IMPORTS ###
import dearpygui.dearpygui as dpg
import os, json, math

### GLOBAL VARIABLES ###

json_slot = { # Template for json 'slots'
    "temp_C": 0,
    "seconds" : 0,
    "start_date": "2000-01-01",
    "start_time": "00:00" # in hours and minutes
}

json_export_data = [] # data to be written to file

INCUBATOR_SCHEDULE_FILE = 'incubator_schedule.json' # constant name of .json file

### GENERAL FUNCTIONS ###

def stage():
    '''reads user selections and exports them to results table'''
    slot = json_slot.copy() # copies template
    slot['temp_C'] = round(dpg.get_value('temperature'), 1)
    slot['duration'] = str(round(dpg.get_value('hours'))) + "h " + str(round(dpg.get_value('minutes'))) + "m " + str(round(dpg.get_value('seconds'))) + "s" # convenient format for readability
    slot['start_date'] = str(dpg.get_value('date_picker')['year']+1875) + ":" + str(dpg.get_value('date_picker')['month']+1).zfill(2) + ":" + str(dpg.get_value('date_picker')['month_day']).zfill(2)
    slot['start_time'] =  str(dpg.get_value('time_picker')['hour']).zfill(2) + ":" +  str(dpg.get_value('time_picker')['min']).zfill(2) # zfill is for formatting and rounding
    slot['seconds'] = round(dpg.get_value('seconds') + 60 * dpg.get_value('minutes') + 60 * 60 * dpg.get_value('hours')) # export total seconds for easier control of Incubator
    
    _table_id = dpg.generate_uuid() # automatically generates item tag instead of hardcoding it to allow for endless slots

    # The below populates the table
    dpg.add_table_row(parent = 'results_table', tag = _table_id)
    dpg.add_text(slot['temp_C'], parent = _table_id)
    dpg.add_text(slot['duration'], parent = _table_id)
    dpg.add_text(slot['start_date'], parent = _table_id)
    dpg.add_text(slot['start_time'], parent = _table_id)

    json_export_data.append(slot) # adds slot to .json file
    

def export_json():
    
    if os.path.exists(INCUBATOR_SCHEDULE_FILE): # if already exists, wipe the .json file
        data = []

    data.append(json_export_data)

    with open(INCUBATOR_SCHEDULE_FILE, "w") as file: # write to .json file
        json.dump(data, file, indent=4)

def button_callback(sender, app_data, user_data):
    ''' Used for debugging'''
    print(f"sender is: {sender}")
    print(f"app_data is: {app_data}")
    print(f"user_data is: {user_data}")

def _log(sender, app_data, user_data):
    ''' Used for debugging'''
    print(f"sender: {sender}, \t app_data: {app_data}, \t user_data: {user_data}")

### CREATE CONTEXT ###

dpg.create_context()

### THEMES ### (need to be defined before creation of widgets)

with dpg.theme() as global_theme: # Applies to widgets, except those targeted by more specific themes

    with dpg.theme_component(dpg.mvAll):
        #dpg.add_theme_color(dpg.mvThemeCol_FrameBg, (255, 140, 100), category=dpg.mvThemeCat_Core)
        dpg.add_theme_style(dpg.mvStyleVar_FrameRounding, 5, category=dpg.mvThemeCat_Core)
        dpg.add_theme_color(dpg.mvThemeCol_Text, (144, 238, 144), category=dpg.mvThemeCat_Core)
        dpg.add_theme_color(dpg.mvThemeCol_TextDisabled, (233, 150, 122), category=dpg.mvThemeCat_Core)

    with dpg.theme_component(dpg.mvButton):
        dpg.add_theme_color(dpg.mvThemeCol_Button, (255, 255, 255), category=dpg.mvThemeCat_Core)
        dpg.add_theme_color(dpg.mvThemeCol_ButtonHovered, (0, 180, 230), category=dpg.mvThemeCat_Core)
        dpg.add_theme_color(dpg.mvThemeCol_ButtonActive, (0, 120, 170), category=dpg.mvThemeCat_Core)
        dpg.add_theme_color(dpg.mvThemeCol_Text, (0, 0, 0), category=dpg.mvThemeCat_Core)

with dpg.theme() as results_theme: # only used for results group
    with dpg.theme_component(dpg.mvAll):
        dpg.add_theme_color(dpg.mvThemeCol_Text, (173, 216, 230), category=dpg.mvThemeCat_Core)

with dpg.theme() as graph_theme: # only used for widgets in the graph group
    with dpg.theme_component(dpg.mvAll):
        dpg.add_theme_color(dpg.mvThemeCol_Text, (255, 255, 255), category=dpg.mvThemeCat_Core)


### MAIN WINDOW ###

dpg.create_viewport(title='Incubator GUI', width=1100, height=850)
with dpg.window(label="Main Window", width =1500, height = 800, pos=(0,0), tag="primary_window", no_collapse=True):
    with dpg.child_window(autosize_x=True):
        with dpg.group(horizontal = True):
            with dpg.child_window(width=400, height=420): # this window is for inputting values
                
                dpg.add_text('Start Time')
                dpg.add_separator()

                dpg.add_time_picker(default_value={'hour': 0, 'min': 0, 'sec': 0}, tag = 'time_picker', hour24 = True)
                dpg.add_separator()

                dpg.add_text('Start Date')
                dpg.add_date_picker(level=dpg.mvDatePickerLevel_Day, default_value={'month_day': 10, 'year':125, 'month':10}, tag ='date_picker')
                dpg.add_separator()

                dpg.add_text('Duration')
                dpg.add_input_float(label="Hours", width = 200, format="%.02f", default_value = 0, step = 1, tag = 'hours')
                dpg.add_input_float(label="Minutes", width = 200, format="%.02f", default_value = 0, step = 1, tag = 'minutes')
                dpg.add_input_float(label="Seconds", width = 200, format="%.02f", default_value = 0, step = 1, tag = 'seconds')
                dpg.add_separator()

                dpg.add_text('Temperature')
                dpg.add_input_float(label="Celsius",width = 200, callback=_log, default_value = 37, format="%.02f", tag = 'temperature', user_data = 'Tempuserdata', step = 1)
                dpg.add_separator()

                dpg.add_button(label="Stage to Results", callback=stage) # exports the slot to the results table
            
            with dpg.child_window(width=650, height=420): # Results window

                dpg.set_item_theme(dpg.last_item(), results_theme)

                dpg.add_text('Results')

                with dpg.table(tag = 'results_table', header_row = True, row_background=True, borders_innerH=True, borders_outerH=True, borders_innerV=True, borders_outerV=True, resizable=True):
                    dpg.add_table_column(label="Temperature (C)")
                    dpg.add_table_column(label="Duration (h/m/s)")
                    dpg.add_table_column(label="Start Date (y/m/d)")
                    dpg.add_table_column(label="Start Time (24hr)")

                dpg.add_button(label="Export json file", callback=export_json)

        with dpg.child_window(width=500, height=400):

            dpg.set_item_theme(dpg.last_item(), graph_theme)

            dpg.add_text('Graph')

            x_values = [i * 0.5 for i in range(0, 201)] # placeholder for graph values
            y_values = [40 + 10 * math.sin(2 * math.pi * i / (201 - 1)) for i in range(201)]

            with dpg.plot(label = 'Incubator Monitor (Work in Progress)', width = -1): # hope to have this read straight from incubator
                dpg.add_plot_legend()
                with dpg.plot_axis(dpg.mvYAxis, label = "Temperature (C)"):
                    dpg.add_scatter_series(x_values, y_values)
                dpg.add_plot_axis(dpg.mvXAxis, label="Time (s)")

dpg.bind_theme(global_theme) # Gives a theme to all widgets not already theme-d

### INITIALIZATION CODE ### (needs to be at the end)
dpg.setup_dearpygui()
dpg.show_viewport()
dpg.start_dearpygui()
dpg.destroy_context()

