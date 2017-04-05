from app import db

class LEDColors(db.Model):
        title = db.Column(db.String(16), index=True)
        id = db.Column(db.Integer,index=True,primary_key = True)
        Red = db.Column(db.Integer,index=True)
        Blue = db.Column(db.Integer,index=True)
        Green = db.Column(db.Integer,index=True)
        timestamp = db.Column(db.DateTime)
        def __repr__(self):
                return 'id :' + str(self.id) + 'Red :' + str(self.Red) + 'Blue :' + str(self.Blue) + 'Green :' + str(self.Green)  

class EnvVariables(db.Model):
        id = db.Column(db.Integer,index=True,primary_key = True)
        Temp = db.Column(db.Float,index=True)
        Humidity = db.Column(db.Float,index=True)
        pH = db.Column(db.Float,index=True);
        timestamp = db.Column(db.DateTime)
