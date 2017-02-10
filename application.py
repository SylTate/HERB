from app import app
application = app
if __name__ == "main":
        application.run(host='0.0.0.0',port=8080,debug=True)
#pp.run(debug=True,host='0.0.0.0')
