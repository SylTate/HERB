from Restful import rest
from app import db
from flask import jsonify
tasks = [
    {
        'id': 1,
        'title': u'Buy groceries',
        'description': u'Milk, Cheese, Pizza, Fruit, Tylenol', 
        'done': False
    },
    {
        'id': 2,
        'title': u'Learn Python',
        'description': u'Need to find a good Python tutorial on the web', 
        'done': False
    }
]
@rest.route('/')
@rest.route('/HERB/api', methods=['GET'])
def get_tasks():
		return jsonify({'tasks': tasks})