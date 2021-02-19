import os
from datetime import datetime


def generate_client_id():
    return os.urandom(16)


def generate_to_string(obj):
    if obj is None:
        value = 'None'
    elif type(obj) == str or type(obj) == int or type(obj) == datetime:
        value = obj
    elif type(obj) == bytes:
        value = obj.hex()
    elif type(obj) == bytearray:
        value = obj.hex()
    elif type(obj) == dict:
        converted_value = {}
        for key, value in obj.items():
            converted_value[key] = generate_to_string(value)
        value = converted_value
    elif type(obj) == list:
        converted_value = []
        for value in obj:
            converted_value.append(generate_to_string(value))
        value = converted_value
    else:
        try:
            value = '%s(%s)' % (
                type(obj).__name__,
                ', '.join('%s=%s' % name_and_key_value_str(item) for item in vars(obj).items())
            )
        except:
            value = ''
    return "{}".format(value)


def name_and_key_value_str(item):
    name, value = item
    value = generate_to_string(value)
    return name, value


def auto_str(cls):
    def __str__(self):
        return generate_to_string(self)

    cls.__str__ = __str__
    return cls
