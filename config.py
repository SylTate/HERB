WTF_CSRF_ENABLED = True
SECRET_KEY = 'you-will-never-guess'
import os
basedir = os.path.abspath(os.path.dirname(__file__))

SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://teamHerb:Seniordesign@herbdb.c6wetbpb49dv.us-west-2.rds.amazonaws.com:3306/HerbDb'
#SQLALCHEMY_DATABASE_URI = 'sqlite:///' + os.path.join(basedir, 'app.db')
SQLALCHEMY_MIGRATE_REPO = os.path.join(basedir, 'db_repository')
