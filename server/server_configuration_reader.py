import os
from abc import ABC


class ServerConfigurationReader(ABC):
    @staticmethod
    def get_port():
        if not os.path.isfile("port.info"):
            raise Exception("port.info does not exist")

        try:
            with open("port.info") as f:
                return int(f.readline().strip())
        except ValueError as ex:
            raise Exception("port.info value error. Error={}".format(ex))
