from flask import render_template,flash,redirect
from app import app
from .forms import LEDColorForm
from app import db,models,comHandler
import datetime
#from app import LEDControl

@app.route('/')
@app.route('/index')
def index():
        color = {'color':100} # fake color 
        data = [{'id': 0,
                 "title":"colors",
                'colors':[0,0,0],
                }]
        return  render_template ('index.html',
                                   title = 'Home',
                                  color=color,
                                   data=data)
@app.route('/dashboard')
def dashboard():
         
       
        return  render_template ('index.html',
                                   temp=50)

@app.route('/setColors',methods = ['GET','POST'])
def setColors():
        form = LEDColorForm()
        if form.validate_on_submit():
                flash('LED Data Posted R=%d, G=%d, B=%d' %
                     (form.Red.data, form.Green.data, form.Blue.data))
               # comHandler = LEDControl.ArduinoLEDCom("3")
                if comHandler.setLEDColors(form.Red.data,form.Green.data, form.Blue.data):

                        return redirect('/index')
        return render_template('LEDColors.html',
                               title = 'Set Colors',
                                form = form)

def generateFakeData():
        colors =models.LEDColors.query.all()
        for color in colors:
                db.session.delete(color)
        db.session.commit()
        for i in range(1,10):
                u = models.LEDColors(Red=i,Blue=i+1,Green=i+2,timestamp=datetime.datetime.utcnow())
                db.session.add(u)
                db.session.commit()


def getColors():
        generateFakeData()
        colors = models.LEDColors.query.all()
        return colors

@app.route("/chart/Red")
def chart():
        colors = getColors()
        labels = []
        values = []
        for color in colors:
                labels.append(color.timestamp)
                print color.Red
                values.append(color.Red)
        return render_template('chart.html',values=values, labels=labels) 

@app.route("/charts")
def charts():
        colors = getColors()
        labels = []
        values = []
        data = [400,500,700,300,299]
        for color in colors:
                labels.append(color.timestamp)
                print color.Red
                values.append(color.Red)
        return render_template('charts.html',values=values, labels=labels, indata=data) 

