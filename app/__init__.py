from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from app import LEDControl
app = Flask(__name__)
app.config.from_object('config')
db = SQLAlchemy(app)
#comHandler = LEDControl.ArduinoLEDCom("3")
from app import views,models

