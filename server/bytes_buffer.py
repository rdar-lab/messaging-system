import os
from abc import ABC, abstractmethod
from socket import socket

from constants import BUFFER_SIZE
from socket_helper import SockHelper


class BytesBuffer(ABC):
    def __len__(self):
        raise NotImplementedError()

    @abstractmethod
    def read(self, size):
        raise NotImplementedError()

    def write_to_socket(self, sock: socket):
        helper = SockHelper(sock)
        while len(self) > 0:
            data = self.read(BUFFER_SIZE)
            helper.write_bytes(data)

    def write_to_file(self, file_name):
        with open(file_name, "wb") as file:
            while len(self) > 0:
                data = self.read(BUFFER_SIZE)
                file.write(data)


class SocketBytesBuffer(BytesBuffer):
    def __len__(self):
        return self.__length

    def read(self, size):
        if self.__length < size:
            size = self.__length

        buffer = self.__helper.read_bytes(size)

        self.__length = self.__length - size
        return buffer

    def __init__(self, sock, length):
        self.__length = length
        self.__sock = sock
        self.__helper = SockHelper(sock)


class FileBytesBuffer(BytesBuffer):
    def __len__(self):
        return self.__length

    def read(self, size):
        if self.__length < size:
            size = self.__length

        buffer = self.__file.read(size)

        self.__length = self.__length - size

        if self.__length == 0:
            self.__file.close()

        return buffer

    def __init__(self, file_name):
        self.__length = os.stat(file_name).st_size
        self.__file = open(file_name, "rb")
