**Янка** - это СОЗНАНИЕ (Система Отслеживания Здоровья, Неполадок, Автоисправления Нарушений, И оповещЕния ) сервера. Не смотря на то, что определение очевидно шутливое - оно достаточно верно отражает суть этой программы. Ну или то, чем она может стать в будущем :)

Янка является ботом для [Tox'а](https://tox.chat/), она может по запросу сообщать информацию о некоторых службах сервера, а так же о состоянии ОС. После запуска она запишет свой Tox ID в журнал. При добавлении этого ID в свой список контактов - необходимо в запросе указать секретную фразу, что бы Янка одобрила запрос. Это является защитой от набивания в друзья сторонних лиц. После этого её можно спросить о состоянии интересущей службы: "_Как там i2pd?_". Или о состоянии ОС: "_Какое LA?_". Янка выделит из вопроса ключевую часть, опросит ОС и выдаст ответ.


Планы на будущие версии:
* ну, пока что это только эхо-бот, так что нужно реализовать описанную систему общения;
* нужно научить Янку передавать сообщения указанным контактам. Это будет неплохой заменой офлайновому обмену сообщениями;
* все те параметры, что прописаны в коде - нужно научиться читать из конфигурационного файла;
* попробовать переписать её на Rust - вроде как он должен быть безопаснее;
* взрастить её до системы мониторинга, которая будет постоянно отслеживать состояние системы и пробовать исправлять ошибки, а не сообщать о текущих делах по запросу.
