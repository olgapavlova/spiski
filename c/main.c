#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>

#define HOST "localhost"
#define USER "root"
#define PSWD ""
#define DBASE "spiski"

int main() {

	MYSQL *mysql = mysql_init(NULL);

	mysql_real_connect(mysql,
											HOST, USER, PSWD, DBASE,
											3306, NULL, 0);

	// Очистить результаты предыдущих попыток распределения по направлениям
	mysql_query(mysql, "UPDATE departments SET counter=0");
	mysql_query(mysql, "UPDATE candidates SET department=NULL");

	// Получить список всех заявлений
	mysql_query(mysql, "SELECT * FROM inbox ORDER BY points DESC, id ASC, priority ASC");
	MYSQL_RES * inbox = mysql_store_result(mysql);

	// Обработать список заявлений
	MYSQL_ROW application;
	while ((application = mysql_fetch_row(inbox))) {

		// Получить запись о кандидате в таблице кандидатов
		char select_candidate[200] = {0};
		strcat(select_candidate, "SELECT * FROM candidates WHERE id='");
		strcat(select_candidate, application[0]);  // id
		strcat(select_candidate, "'");

		mysql_query(mysql, select_candidate);
		MYSQL_RES *candidate_search_result = mysql_store_result(mysql);
		MYSQL_ROW candidate = mysql_fetch_row(candidate_search_result);

		if (candidate[1] == NULL) {  // ещё не распределён

			// Получить информацию о направлении, указанном в заявлении
			char select_department[200] = {0};
			strcat(select_department, "SELECT * from departments WHERE id='");
			strcat(select_department, application[3]);
			strcat(select_department, "'");
			
			mysql_query(mysql, select_department);
			MYSQL_RES *department_search_result = mysql_store_result(mysql);
			MYSQL_ROW department = mysql_fetch_row(department_search_result);

			// Если есть места — зачислить кандидата на направление
			int department_budget;
			sscanf(department[2], "%d", &department_budget);

			int department_counter;
			sscanf(department[4], "%d", &department_counter);

			if (department_budget > department_counter) {
				// Увеличиваем счётчик зачисленных на направление (можно обойтись сложным SQL-запросом)
				char increase_counter[200] = {0};
				strcat(increase_counter, "UPDATE departments SET counter = counter + 1 WHERE id='");
				strcat(increase_counter, application[3]);
				strcat(increase_counter, "'");

				mysql_query(mysql, increase_counter);

				// Записываем кандидата на направление
				char set_department[200] = {0};
				strcat(set_department, "UPDATE candidates SET department='");
				strcat(set_department, application[3]);
				strcat(set_department, "', points='");
				strcat(set_department, application[1]);
				strcat(set_department, "', priority='");
				strcat(set_department, application[2]);
				strcat(set_department, "' WHERE id='");
				strcat(set_department, application[0]);
				strcat(set_department, "'");

				mysql_query(mysql, set_department);
			}

			mysql_free_result(department_search_result);
		}

		mysql_free_result(candidate_search_result);
	}

	// Освобождаем ресурсы
	mysql_free_result(inbox);
  mysql_close(mysql);
}
