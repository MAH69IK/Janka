#include <tox/tox.h>

int main() {
	TOX_ERR_NEW stato;
	// Возможно вынести инициализацию в отдельнюу малюсенькую ф-цию, это позволит повторять попытку
	Tox *tox = tox_new(NULL, &stato);
	switch (stato) {
		case TOX_ERR_NEW_MALLOC:
			fprintf (stderr, "Иницилизация не удалась из-за невозможности выделить память (%s).\n", stato);
			// Вывести информацию о свободной памяти
			return 1;
			break;
		case TOX_ERR_NEW_PORT_ALLOC:
			fprintf (stderr, "Иницилизация не удалась из-за невозможности использования порта (%s). Возможно порт занят (другим экземпляром Tox'а) или не достаточно прав.\n", stato);
			// Вывести сведения о занятости порта и/или правах. Попробовать использовать порты с номерами больше/меньше (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_PROXY_BAD_TYPE:
			fprintf (stderr, "Иницилизация не удалась - не верно указан тип прокси (%s).\n", stato);
			// Вывести информацию о допустимых значениях и указанном, попробовать повторить без прокси (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_PROXY_BAD_HOST;
			fprintf (stderr, "Иницилизация не удалась - не верно указан (или вообще пропущен) адрес прокси при включённом использовании прокси (%s).\n", stato);
			// Вывести информацию об указанном значении, попробовать повторить без прокси (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_PROXY_BAD_PORT:
			fprintf (stderr, "Иницилизация не удалась - не верно указан порт прокси (%s).\n", stato);
			// Вывести информацию об указанном значении, попробовать повторить без прокси (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_LOAD_ENCRYPTED:
			fprintf (stderr, "Иницилизация не удалась - загружаемые данные зашифрованы (%s).\n", stato);
			// Посмотреть подробнее из-за чего возникает ошибка, что именно зашифровано
			return 1;
			break;
		case TOX_ERR_NEW_LOAD_BAD_FORMAT:
			fprintf (stderr, "Иницилизация не удалась из-за невозможности загрузки данных (%s). Возможно загружаемые данные принадлежат старой версии Tox'а или данные повреждены.\n", stato);
			// Проверить подробнее что за данные, условия ошибки, описат подробнее
			return 1;
			break;
		case TOX_ERR_NEW_OK;
			// Тут всё хорошо
			break
	}

	tox_callback_self_connection_status(tox handle_self_connection_status);
	tox_callback_friend_request(tox, handle_friend_request);
	tox_callback_friend_message(tox, handle_friend_message);

	return 0;
}
