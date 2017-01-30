from flask import Flask
from flask.ext.sqlalchemy import SQLAlchemy
import LEDControl
app = Flask(__name__)
app.config.from_object('config')
db = SQLAlchemy(app)

comHandler = LEDControl.ArduinoLEDCom("3")
from app import views,models

