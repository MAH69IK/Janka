#include <tox/tox.h>

static void handle_friend_request(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data) {
	// Нужно проверить, что секретный код, переданный в тексте запросе, совпадает с секретным кодом, сохранённом в конфиге, что бы не добавлять всех подряд.
	TOX_ERR_FRIEND_ADD err_friend_add;
	tox_friend_add_norequest(tox, public_key, &err_friend_add);
	if (err_friend_add != TOX_ERR_FRIEND_ADD_OK) {
		fprintf(stderr, "unable to add friend: %d\n", err_friend_add);
	}
}

static void handle_friend_message(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data) {
	// Пример из документации, который просто отсылает сообщение назад. Нужно будет переделать под обработку полученных сообщений.
	TOX_ERR_FRIEND_SEND_MESSAGE err_send;
	tox_friend_send_message(tox, friend_number, type, message, length, &err_send);
	if (err_send != TOX_ERR_FRIEND_SEND_MESSAGE_OK) {
		fprintf(stderr, "unable to send message back to friend %d: %d\n", friend_number, err_send);
	}
}

int main() {
	TOX_ERR_NEW rezulto_new;
	// Возможно вынести инициализацию в отдельнюу малюсенькую ф-цию, это позволит повторять попытку

	// Создаём экземпляр Tox'а.
	Tox *tox = tox_new(NULL, &rezulto_new);
	switch (rezulto_new) {
		case TOX_ERR_NEW_MALLOC:
			fprintf (stderr, "Иницилизация не удалась из-за невозможности выделить память (%s).\n", rezulto_new);
			// Вывести информацию о свободной памяти
			return 1;
			break;
		case TOX_ERR_NEW_PORT_ALLOC:
			fprintf (stderr, "Иницилизация не удалась из-за невозможности использования порта (%s). Возможно порт занят (другим экземпляром Tox'а) или не достаточно прав.\n", rezulto_new);
			// Вывести сведения о занятости порта и/или правах. Попробовать использовать порты с номерами больше/меньше (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_PROXY_BAD_TYPE:
			fprintf (stderr, "Иницилизация не удалась - не верно указан тип прокси (%s).\n", rezulto_new);
			// Вывести информацию о допустимых значениях и указанном, попробовать повторить без прокси (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_PROXY_BAD_HOST;
			fprintf (stderr, "Иницилизация не удалась - не верно указан (или вообще пропущен) адрес прокси при включённом использовании прокси (%s).\n", rezulto_new);
			// Вывести информацию об указанном значении, попробовать повторить без прокси (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_PROXY_BAD_PORT:
			fprintf (stderr, "Иницилизация не удалась - не верно указан порт прокси (%s).\n", rezulto_new);
			// Вывести информацию об указанном значении, попробовать повторить без прокси (настраивать через конфиг)
			return 1;
			break;
		case TOX_ERR_NEW_LOAD_ENCRYPTED:
			fprintf (stderr, "Иницилизация не удалась - загружаемые данные зашифрованы (%s).\n", rezulto_new);
			// Посмотреть подробнее из-за чего возникает ошибка, что именно зашифровано
			return 1;
			break;
		case TOX_ERR_NEW_LOAD_BAD_FORMAT:
			fprintf (stderr, "Иницилизация не удалась из-за невозможности загрузки данных (%s). Возможно загружаемые данные принадлежат старой версии Tox'а или данные повреждены.\n", rezulto_new);
			// Проверить подробнее что за данные, условия ошибки, описат подробнее
			return 1;
			break;
		case TOX_ERR_NEW_OK;
			// Тут всё хорошо
			break
	}

	// Получаем наш Tox ID.
	uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
	tox_self_get_address(tox, tox_id_bin);

	// Ф-ция tox_self_get_address() возвращает Tox ID в бинарном виде, преобразовываем его в шестнадцатиричный.
	char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
	sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));

	// Ф-ция sodium_bin2hex() вернула идентификатор с буквами нижнего регистра, но Tox ID записывается с использованием верхнего регистра, меняем.
	for (size_t i = 0; i < sizeof(tox_id_hex) - 1; i++) {
		tox_id_hex[i] = toupper(tox_id_hex[i]);
	}

	// Записываем наш идентификатор в журнал.
	fprintf (stdout, "Мой Tox ID: %s.\n", tox_id_hex);

	// Задаём имя нашему экземпляру Tox'а.
	char const *const nomo = "Янка";
	// Если имя будет браться из конфига, то в 4-ом параметре надо передавать переменную типа TOX_ERR_SET_INFO и затем проверять её (либо самостоятельно проверять длину имени вначале).
	tox_self_set_name(tox, nomo, strlen(nomo), NULL);

	// Регистрируем ф-ции обратного вызова.
	// handle_self_connection_status - для обработки потери соединения;
	// handle_friend_request - для обработки входящих запросов на добавление в список контактов;
	// handle_friend_message - для обработки входящих сообщений.
	// Возможно в будущем добавить настраиваемую опцию отправки сообщения при появлении контакта в Сети.
	tox_callback_self_connection_status(tox, handle_self_connection_status);
	tox_callback_friend_request(tox, handle_friend_request);
	tox_callback_friend_message(tox, handle_friend_message);

	// Подключаемся к Сети.
	// Результат - bool, проверить.
	// Порт и ключ преобразовать.
	tox_bootstrap(tox, "2a00:7a60:0:746b::3", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43", TOX_ERR_BOOTSTRAP rezulto_bootstrap);

	while (true) {
		usleep(1000 * tox_iteration_interval(tox));
		tox_iterate(tox, NULL);
	}

	return 0;
}
