from flask import render_template,flash,redirect,url_for,Response,request,jsonify
from app import app
from .forms import LEDColorForm,WaterCycleForm
from app import db,models
import datetime,itertools,time
from app import ArduinoCom
comHandler = ArduinoCom.ArduinoCom("3")

@app.route('/')
@app.route('/index')
def index():
        color = {'color':100} # fake color
        data = {
                "labels": ["January", "February", "March", "April", "May", "June", "July"],
                    "datasets": [
             {
            "label": "My First dataset",
            "fillColor": "rgba(220,220,220,0.2)",
            "strokeColor": "rgba(220,220,220,1)",
            "pointColor": "rgba(220,220,220,1)",
            "pointStrokeColor": "#fff",
            "pointHighlightFill": "#fff",
            "pointHighlightStroke": "rgba(220,220,220,1)",
            "data": [65, 59, 80, 81, 56, 55, 40]
        },
        {
            "label": "My Second dataset",
            "fillColor": "rgba(151,187,205,0.2)",
            "strokeColor": "rgba(151,187,205,1)",
            "pointColor": "rgba(151,187,205,1)",
            "pointStrokeColor": "#fff",
            "pointHighlightFill": "#fff",
            "pointHighlightStroke": "rgba(151,187,205,1)",
            "data": [28, 48, 40, 19, 86, 27, 90]
        }
        ]
        }
        return  render_template ('index3.html',
                                  title = 'Home',
                                  color=color,
                                    indata = data)

@app.route('/getEnvironmentData',methods = ['GET'])
def getEnvironmentData ():
    temp = [200,300,100,50,700,250,100];
    humidity = [240,700,100,510,400,250,100];
    return jsonify({"temp":temp,"humidity":humidity})

@app.route('/getStream')
def streamer():
        def events():
           #for i in range(0,100):
            #    j = i % 2
           while(True):
                i = float(comHandler.getTemp())
                j = float(comHandler.getHumidity())

                ##yield "data: %s %s\n\n" % (c,comHandler.getTemp() )
                #yield "data: %s %s\n\n" % (c,"hello" )
                #print "event:liveData \n\n data:{temp:%s,humidity:%s}\n\n" % (c,"hello" )
                #time.sleep(.1)  # an artificial delay)
                #yield "data:  %s %s\n\n event: liveData\n data: {temp: %s,humidity: %s}\n\n" % (c,"hello" ,c,"hello" )
                output = "data: {\ndata: \"temp\": %f,\ndata: \"humidity\": %f \ndata:}\n\n"%(i,j)
                print output
                yield output
                time.sleep(4)  # an artificial delay
        return Response(events(), content_type='text/event-stream')




@app.route('/dashboard')
def dashboard():


        return  render_template ('index.html',
                                   temp=50)

@app.route('/setColors',methods = ['GET','POST'])
def setColors():
        form = LEDColorForm()
        if form.validate_on_submit():
                comHandler = ArduinoCom.ArduinoCom("3")
                if comHandler.setLEDColors(form.Red.data,form.Green.data, form.Blue.data):
                    flash('LED Data Posted R=%d, G=%d, B=%d' %
                     (form.Red.data, form.Green.data, form.Blue.data))
               # comHandler = LEDControl.ArduinoLEDCom("3")
                #uncomment
                #if comHandler.setLEDColors(form.Red.data,form.Green.data, form.Blue.data):
#uncomment

       # return render_template('LEDColors.html',
                           #    title = 'Set Colors',
                             #   form = form)
                else:
                     flash('LED Data Posting failed')
                comHandler.ser.close();
                return redirect('/index')
        return render_template('LEDColors.html',
                             title = 'Set Colors',
                             form = form)

@app.route('/setLightPeriod',methods = ['GET','POST'])
def setLightPeriod() :
    print(request.json)
    return jsonify({"light_time":request.json['light_time']})

@app.route('/setColorsJSON', methods = ['POST'])
def setColorsHeadless() :
    if not request.json or not ('Red' in request.json  or 'Green' in request.json or 'Blue' in request.json) :
        print(request.json)
        abort(400)
    if comHandler.setLEDColors(request.json['Red'],request.json['Green'], request.json['Blue']):
        print(request.json)
    return jsonify({"Red":request.json['Red'], "Green":request.json['Green'],"Blue":request.json['Blue']})

@app.route('/Slider')
def RenderSlider():
    return render_template('SliderLayout.html')

@app.route('/waterCycle',methods = ['GET','POST'])
def waterCycle():
        form = WaterCycleForm()
        if form.validate_on_submit():
                comHandler = ArduinoCom.ArduinoCom("3")
                if comHandler.setWaterCycle(form.emptyTime.data,form.fullTime.data):
                    flash('water cycle time set empty=%d, full=%d' %
                     (form.emptyTime.data,form.fullTime.data))
                return redirect('/index')
        return render_template('waterCycle.html',
                             title = 'Set Water Cylce',
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
                print( color.Red)
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
                print (color.Red)
                values.append(color.Red)
        return render_template('charts.html',values=values, labels=labels, indata=data)

