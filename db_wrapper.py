from database import my_db

class db_wrapper:
    def check_if_rp_exists(this, registration_plate : str):
        if(len(my_db.execute_query(f"SELECT approved_registration_plates.plate_num FROM approved_registration_plates WHERE plate_num = '{registration_plate}'")) == 1):
            return 1
        else:
            return 0
    
    def get_report_1(this):
        return my_db.execute_query("SELECT count(*) as \"entries\", EXTRACT( YEAR FROM access_logs.log_date ) as \"year\" from access_logs \
            GROUP BY EXTRACT( YEAR FROM access_logs.log_date ) ORDER BY EXTRACT( YEAR FROM access_logs.log_date ) ASC")
    
    def get_report_2(this):
        return my_db.execute_query("SELECT count(*) as \"entries\", EXTRACT( MONTH FROM access_logs.log_date ) as \"month\" from access_logs \
            WHERE EXTRACT( YEAR FROM access_logs.log_date ) = 2022\
            GROUP BY  EXTRACT( MONTH FROM access_logs.log_date ) ORDER BY EXTRACT( MONTH FROM access_logs.log_date ) ASC ")
    
    def get_report_3(this):
        return my_db.execute_query("SELECT access_logs.plate_num from access_logs WHERE DATE(access_logs.log_date) = CURRENT_DATE")

    def get_report_4(this):
        return my_db.execute_query("SELECT approved_registration_plates.plate_num FROM approved_registration_plates ORDER BY approved_registration_plates.plate_num ASC")

    def get_report_5(this):
        return my_db("SELECT count(access_logs.log_id) as \"cars parked\" from approved_registration_plates, access_logs\
            WHERE DATE(access_logs.log_date) = CURRENT_DATE AND approved_registration_plates.plate_num = access_logs.plate_num")