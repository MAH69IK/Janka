#include <tox/tox.h>

int main() {
	TOX_ERR_NEW stato;
	// Возможно вынести инициализацию в отдельнюу малюсенькую ф-цию, это позволит повторять попытку
	Tox *tox = tox_new(NULL, &stato);
	/*if (stato != TOX_ERR_NEW_OK) {
		fprintf (stderr, "Иницилизация не удалась из-за ошибки: \"%s\"\n", stato);
		exit(1);
	}*/
	switch (stato) {
		case TOX_ERR_NEW_LOAD_BAD_FORMAT:
    /**
     * The data format was invalid. This can happen when loading data that was
     * saved by an older version of Tox, or when the data has been corrupted.
     * When loading from badly formatted data, some data may have been loaded,
     * and the rest is discarded. Passing an invalid length parameter also
     * causes this error.
     */
			break;
		case TOX_ERR_NEW_LOAD_ENCRYPTED:
    /**
     * The byte array to be loaded contained an encrypted save.
     */
			break;
		case TOX_ERR_NEW_PROXY_BAD_TYPE:
    /**
     * proxy_type was invalid.
     */
			break;
		case TOX_ERR_NEW_PROXY_BAD_PORT:
    /**
     * proxy_type was valid, but the proxy_port was invalid.
     */
			break;
		case TOX_ERR_NEW_PROXY_BAD_HOST;
    /**
     * proxy_type was valid but the proxy_host passed had an invalid format
     * or was NULL.
     */
			break;
		case TOX_ERR_NEW_MALLOC:
			// Вывести сообщение о нехватки памяти и вывести информацию о свободной памяти
			break;
		case TOX_ERR_NEW_PORT_ALLOC:
			// Вывести сообщение о невозможности назначения порта в следствии его занятости или нехватки прав. Вывести сведения о занятости и/или правах. Попробовать использовать порты на 10 больше/меньше (настраиать через конфиг)
			break;
		// Продолжить с другими возможными типами ошибок
		case TOX_ERR_NEW_OK;
			// Тут всё хорошо
			break
	}

	return 0;
}
