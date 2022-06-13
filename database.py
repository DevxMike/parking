import psycopg2

class db_connector:
    def __init__(this, db_credentials : list, db_details : list):
        assert(len(db_credentials) == 2)
        assert(len(db_details) == 2)
        
        this.connection = psycopg2.connect(f"dbname='{db_details[0]}' user = '{db_credentials[0]}' host = '{db_details[1]}' password = '{db_credentials[1]}'")

    def execute_query(this, query):
        cur = this.connection.cursor()
        cur.execute(query)
        return cur.fetchall()

my_db = db_connector(['postgres', 'qwerty'], ['parking', 'localhost'])