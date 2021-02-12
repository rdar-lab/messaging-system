import logging
from sys import stdout

from in_mem_storage_manager import InMemStorageManager
from server_communication_manager import ServerCommunicationManager
from server_configuration_reader import ServerConfigurationReader

storage_manager = InMemStorageManager()

if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO, stream=stdout,
                        format="%(asctime)s - %(levelname)s - %(filename)s:%(lineno)d - %(message)s")

    logger = logging.getLogger(__name__)
    logger.info("Log system init")
    try:
        port = ServerConfigurationReader.get_port()
        commManager = ServerCommunicationManager(storage_manager, port)
        commManager.start_server()
    except Exception:
        logger.exception("Error starting server")