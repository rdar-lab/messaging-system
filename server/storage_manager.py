from abc import ABC, abstractmethod

from model import Client, Message


class StorageManager(ABC):
    @abstractmethod
    def get_all_clients(self) -> dict:
        raise NotImplementedError()

    @abstractmethod
    def get_client(self, client_id) -> Client:
        raise NotImplementedError()

    @abstractmethod
    def add_client(self, client_name, client_public_key) -> Client:
        raise NotImplementedError()

    @abstractmethod
    def update_last_seen(self, client_id):
        raise NotImplementedError()

    @abstractmethod
    def remove_client(self, client_id):
        raise NotImplementedError()

    @abstractmethod
    def add_message(self, to_client_id, from_client_id, message_type, message_content) -> Message:
        raise NotImplementedError()

    @abstractmethod
    def get_messages(self, client_id, limit=0) -> list:
        raise NotImplementedError()

    @abstractmethod
    def remove_messages(self, messages_to_remove):
        raise NotImplementedError()
