import logging
import os
from abc import ABC

from bytes_buffer import BytesBuffer, FileBytesBuffer
from constants import FILE_STORE_FOLDER

_logger = logging.getLogger(__name__)


class FileStore(ABC):
    """
    The file store allows to easily store and get files
    The operations are implemented with bytes buffer to allow support for big files
    """

    @staticmethod
    def store_file(file_name, bytes_buffer: BytesBuffer):
        if not os.path.exists(FILE_STORE_FOLDER):
            os.makedirs(FILE_STORE_FOLDER)

        _logger.info("Storing file: {}".format(file_name))
        bytes_buffer.write_to_file(FILE_STORE_FOLDER + file_name)

    @staticmethod
    def read_file(file_name):
        _logger.info("Reading file: {}".format(file_name))

        if os.path.exists(FILE_STORE_FOLDER + file_name):
            return FileBytesBuffer(FILE_STORE_FOLDER + file_name)
        else:
            _logger.warning("File not found")
            return None

    @staticmethod
    def remove_file(file_name):
        _logger.info("Removing file: {}".format(file_name))
        os.remove(FILE_STORE_FOLDER + file_name)
