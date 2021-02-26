import logging
from sys import stdout

from db_storage_manager import DbStorageManager
from server_communication_manager import ServerCommunicationManager
from server_configuration_reader import ServerConfigurationReader

# MAIN start .py file for the server

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO, stream=stdout,
                        format="%(asctime)s - %(levelname)s - %(filename)s:%(lineno)d - %(message)s")

    logger = logging.getLogger(__name__)
    logger.info("Log system init")

    # noinspection PyBroadException
    try:
        storage_manager = DbStorageManager()
        port = ServerConfigurationReader.get_port()
        commManager = ServerCommunicationManager(storage_manager, port)
        commManager.start_server()
    except Exception:
        logger.exception("Error starting server")
