import db_wrapper

class read_only_ui:
    def __init__(this):
        this.db = db_wrapper.db_wrapper()

    def get_reports(this) -> list:
        r1 = this.db.get_report_1()
        r2 = this.db.get_report_2()
        r3 = this.db.get_report_3()
        r4 = this.db.get_report_4()
        r5 = this.db.get_report_5()

        return [r1, r2, r3, r4, r5]