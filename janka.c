#include <tox/tox.h>

// Почему static void?
static void handle_friend_request(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data) {
	// Запрос принимается только от тех, кто прислал в запросе секретную фразу.
	if (message) {
		// Конечно же это надо будет читать из конфигурационного файла.
		const char *const sekreto = "werx#@978";
		if (strcmp(message, sekreto) == 0) {
			TOX_ERR_FRIEND_ADD rezulto_friend_add_norequest;

			tox_friend_add_norequest(tox, public_key, &rezulto_friend_add_norequest);
			// Переделать на более информативную обработку ошибок, как в tox_new.
			if (rezulto_friend_add_norequest != TOX_ERR_FRIEND_ADD_OK) {
				fprintf(stderr, "Не удалось добавить контакт (%d).\n", rezulto_friend_add_norequest);
			}
		}
		else {
			fprintf(stderr, "Неверная секретная фраза. Переданное значение: \"%s\".\n", message);
		}
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

	// Записываем наш Tox ID.
	{
		// Получаем наш Tox ID.
		uint8_t tox_id_2[TOX_ADDRESS_SIZE];
		tox_self_get_address(tox, tox_id_2);

		// Ф-ция tox_self_get_address() возвращает Tox ID в бинарном виде, преобразовываем его в шестнадцатиричный.
		char tox_id_16[TOX_ADDRESS_SIZE * 2 + 1];
		sodium_bin2hex(tox_id_16, sizeof(tox_id_16), tox_id_2, sizeof(tox_id_2));

		// Ф-ция sodium_bin2hex() вернула идентификатор с буквами нижнего регистра, но Tox ID записывается с использованием верхнего регистра, меняем.
		for (size_t i = 0; i < sizeof(tox_id_16) - 1; i++) {
			tox_id_16[i] = toupper(tox_id_16[i]);
		}

		// Записываем наш идентификатор в журнал.
		fprintf (stdout, "Мой Tox ID: %s.\n", tox_id_16);
	}

	// Задаём имя нашему экземпляру Tox'а.
	{
		const char *const nomo = "Янка";
		// Если имя будет браться из конфига, то в 4-ом параметре надо передавать переменную типа TOX_ERR_SET_INFO и затем проверять её (либо самостоятельно проверять длину имени вначале).
		tox_self_set_name(tox, nomo, strlen(nomo), NULL);
	}

	// Регистрируем ф-ции обратного вызова.
	// handle_self_connection_status - для обработки потери соединения;
	// handle_friend_request - для обработки входящих запросов на добавление в список контактов;
	// handle_friend_message - для обработки входящих сообщений.
	// Возможно в будущем добавить настраиваемую опцию отправки сообщения при появлении контакта в Сети.
	tox_callback_self_connection_status(tox, handle_self_connection_status);
	tox_callback_friend_request(tox, handle_friend_request);
	tox_callback_friend_message(tox, handle_friend_message);

	// Подключаемся к Сети.
	{
		typedef struct DHT_retnodo {
			const char *IP;
			uint16_t pordo;
			const char id_16[TOX_PUBLIC_KEY_SIZE * 2 + 1];
			unsigned char id_2[TOX_PUBLIC_KEY_SIZE]; //Почему unsigned?
		} DHT_retnodo;
 
		DHT_retnodo retnodoj[] = {
			{"2a00:7a60:0:746b::3", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43", {0}}
		};

		TOX_ERR_BOOTSTRAP rezulto_bootstrap;

		for (size_t i = 0; i < sizeof(retnodoj) / sizeof(DHT_retnodo); i++) {
			sodium_hex2bin(retnodoj[i].id_2, sizeof(retnodoj[i].id_2), retnodoj[i].id_16, sizeof(retnodoj[i].id_16) - 1);
			// Результат - bool, его надо проверять.
			tox_bootstrap(tox, retnodoj[i].IP, retnodoj[i].pordo, retnooj[i].id_16, &rezulto_bootstrap);
}
	}

	while (true) {
		usleep(1000 * tox_iteration_interval(tox));
		tox_iterate(tox, NULL);
	}

	return 0;
}
