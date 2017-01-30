from app import db

class LEDColors(db.Model):
        title = db.Column(db.String, index=True)
        id = db.Column(db.Integer,index=True,primary_key = True)
        Red = db.Column(db.Integer,index=True)
        Blue = db.Column(db.Integer,index=True)
        Green = db.Column(db.Integer,index=True)
        timestamp = db.Column(db.DateTime)

class EnvVariables(db.Model):
        title = db.Column(db.String, index=True)
        id = db.Column(db.Integer,index=True,primary_key = True)
        Temp = db.Column(db.Float,index=True)
        Humidity = db.Column(db.Float,index=True)
        timestamp = db.Column(db.DateTime)
