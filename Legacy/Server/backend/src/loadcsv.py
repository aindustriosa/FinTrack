import csv
import MySQLdb
import random
from datetime import datetime
from raw2si import year_parse, month_parse, day_parse
from raw2si import hour_parse, minute_parse, second_parse, array_to_datetime
from raw2si import degE7_parse
from raw2si import voltRaw_to_avg, voltRaw_to_std, ampRaw_to_avg, ampRaw_to_std
from raw2si import ldrRaw_to_avg, ldrRaw_to_std, presRaw_to_avg, presRaw_to_std
from raw2si import accRaw_to_avg, accRaw_to_std, gyrRaw_to_avg, gyrRaw_to_std
from raw2si import magRaw_to_avg, magRaw_to_std

mydb = MySQLdb.connect(host='localhost',
    user='fintrackuser',
    passwd='<password>',
    db='fintrackdb')
cursor = mydb.cursor()

csv_data = csv.reader(file('input.csv'))

name='Barco A';

firstline = True
for row in csv_data:
    if firstline:        #skip first line
        firstline = False
        continue

    cursor.execute('INSERT INTO dataraw (identifier,name,date,timestamp_ms,year,month,day,hour,min,sec,lat_E7,lon_E7,alt_cm,lat_err_cm,lon_err_cm,alt_err_cm,volt_avg,volt_std,amp_avg,amp_std,ldr_avg,ldr_std,pres_avg_hpa,pres_std,acx_avg,acx_std,acy_avg,acy_std,acz_avg,acz_std,gyx_avg,gyx_std,gyy_avg,gyy_std,gyz_avg,gyz_std,mgx_avg,mgx_std,mgy_avg,mgy_std,mgz_avg,mgz_std) VALUES ( \
        %(identifier)s, \
        %(name)s, \
        %(date)s, \
        %(timestamp_ms)s, \
        %(year)s, \
        %(month)s, \
        %(day)s, \
        %(hour)s, \
        %(min)s, \
        %(sec)s, \
        %(lat_E7)s, \
        %(lon_E7)s, \
        %(alt_cm)s, \
        %(lat_err_cm)s, \
        %(lon_err_cm)s, \
        %(alt_err_cm)s, \
        %(volt_avg)s, \
        %(volt_std)s, \
        %(amp_avg)s, \
        %(amp_std)s, \
        %(ldr_avg)s, \
        %(ldr_std)s, \
        %(pres_avg_hpa)s, \
        %(pres_std)s, \
        %(acx_avg)s, \
        %(acx_std)s, \
        %(acy_avg)s, \
        %(acy_std)s, \
        %(acz_avg)s, \
        %(acz_std)s, \
        %(gyx_avg)s, \
        %(gyx_std)s, \
        %(gyy_avg)s, \
        %(gyy_std)s, \
        %(gyz_avg)s, \
        %(gyz_std)s, \
        %(mgx_avg)s, \
        %(mgx_std)s, \
        %(mgy_avg)s, \
        %(mgy_std)s, \
        %(mgz_avg)s, \
        %(mgz_std)s)' ,

    {'identifier': row[0],
        'name': name,
        'date': array_to_datetime(row[2], row[3], row[4],row[5],row[6],row[7]),
        'timestamp_ms': int(row[1]),
        'year': year_parse(row[2]),
        'month': month_parse(row[3]),
        'day': day_parse(row[4]),
        'hour': hour_parse(row[5]),
        'min': minute_parse(row[6]),
        'sec': second_parse(row[7]),
        'lat_E7': degE7_parse(row[8]),
        'lon_E7': degE7_parse(row[9]),
        'alt_cm': int(row[10]),
        'lat_err_cm': int(row[11]),
        'lon_err_cm': int(row[12]),
        'alt_err_cm': int(row[13]),
        'volt_avg': voltRaw_to_avg(row[14]),
        'volt_std': voltRaw_to_std(row[15],row[14]),
        'amp_avg': ampRaw_to_avg(row[16]),
        'amp_std': ampRaw_to_std(row[17],row[16]),
        'ldr_avg': ldrRaw_to_avg(row[18]),
        'ldr_std': ldrRaw_to_std(row[19],row[18]),
        'pres_avg_hpa': presRaw_to_avg(row[20]),
        'pres_std': presRaw_to_std(row[21],row[20]),
        'acx_avg': accRaw_to_avg(row[22]),
        'acx_std': accRaw_to_std(row[23],row[22]),
        'acy_avg': accRaw_to_avg(row[24]),
        'acy_std': accRaw_to_std(row[25],row[24]),
        'acz_avg': accRaw_to_avg(row[26]),
        'acz_std': accRaw_to_std(row[27],row[26]),
        'gyx_avg': gyrRaw_to_avg(row[28]),
        'gyx_std': gyrRaw_to_std(row[29],row[28]),
        'gyy_avg': gyrRaw_to_avg(row[30]),
        'gyy_std': gyrRaw_to_std(row[31],row[30]),
        'gyz_avg': gyrRaw_to_avg(row[32]),
        'gyz_std': gyrRaw_to_std(row[33],row[32]),
        'mgx_avg': magRaw_to_avg(row[34]),
        'mgx_std': magRaw_to_std(row[35],row[34]),
        'mgy_avg': magRaw_to_avg(row[36]),
        'mgy_std': magRaw_to_std(row[37],row[36]),
        'mgz_avg': magRaw_to_avg(row[38]),
        'mgz_std': magRaw_to_std(row[39],row[38])
        });

mydb.commit()
cursor.close()
print "Done"
