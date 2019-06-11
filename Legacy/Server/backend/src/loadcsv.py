import csv
import MySQLdb
import random
from datetime import datetime

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
        'date': datetime(int(row[2]), int(row[3]), int(row[4]), int(row[5]), int(row[6]), int(row[7])),
        'timestamp_ms': int(row[1]),
        'year': int(row[2]),
        'month': int(row[3]),
        'day': int(row[4]),
        'hour': int(row[5]),
        'min': int(row[6]),
        'sec': int(row[7]),
        'lat_E7': round(float(row[8])/10000000,6),
        'lon_E7': round(float(row[9])/10000000,6),
        'alt_cm': int(row[10]),
        'lat_err_cm': int(row[11]),
        'lon_err_cm': int(row[12]),
        'alt_err_cm': int(row[13]),
        'volt_avg': int(row[14])+random.randint(0, 100),
        'volt_std': int(row[15]),
        'amp_avg': int(row[16])+random.randint(0, 200),
        'amp_std': int(row[17]),
        'ldr_avg': int(row[18]),
        'ldr_std': int(row[19]),
        'pres_avg_hpa': int(row[20]),
        'pres_std': int(row[21]),
        'acx_avg': int(row[22]),
        'acx_std': int(row[23]),
        'acy_avg': int(row[24]),
        'acy_std': int(row[25]),
        'acz_avg': int(row[26]),
        'acz_std': int(row[27]),
        'gyx_avg': int(row[28]),
        'gyx_std': int(row[29]),
        'gyy_avg': int(row[30]),
        'gyy_std': int(row[31]),
        'gyz_avg': int(row[32]),
        'gyz_std': int(row[33]),
        'mgx_avg': int(row[34]),
        'mgx_std': int(row[35]),
        'mgy_avg': int(row[36]),
        'mgy_std': int(row[37]),
        'mgz_avg': int(row[38]),
        'mgz_std': int(row[39])
        });

mydb.commit()
cursor.close()
print "Done"
