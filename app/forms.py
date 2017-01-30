from flask_wtf import Form
from wtforms import StringField, BooleanField,IntegerField
from wtforms.validators import DataRequired

class LEDColorForm(Form):
            Red = IntegerField('Red', validators=[DataRequired()])
            Green = IntegerField('Green', validators=[DataRequired()])
            Blue = IntegerField('Blue', validators=[DataRequired()])
