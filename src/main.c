#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>

int main() {

	MYSQL *mysql = mysql_init(NULL);

	if (!mysql_real_connect(mysql,
				"localhost", "root", "", "spiski", 3306, NULL, 0)) {
    			fprintf(stderr,
						"Failed to connect to database: Error: %s\n",
						mysql_error(mysql));
	}

	mysql_query(mysql, "update departments set counter=0");
	MYSQL_RES *clear_counter = mysql_store_result(mysql);


	mysql_query(mysql, "SELECT * FROM inbox order by points desc, id asc, priority asc");

	MYSQL_RES *result = mysql_store_result(mysql);


	MYSQL_ROW row;

	while ((row = mysql_fetch_row(result))) {

		
		char select_candidate[200] = {0};
				strcat(select_candidate, "SELECT * from candidates WHERE id='");
				strcat(select_candidate, row[0]);
				strcat(select_candidate, "'");

				mysql_query(mysql, select_candidate);
				MYSQL_RES *can = mysql_store_result(mysql);
				MYSQL_ROW c = mysql_fetch_row(can);

		if (c[1]) {
			// printf("%s\t%s\n", c[0], c[1]);
		} else {
			char select_department[200] = {0};
				strcat(select_department, "SELECT * from departments WHERE id='");
				strcat(select_department, row[3]);
				strcat(select_department, "'");
			
				//printf("%s\n", select_department); 

				mysql_query(mysql, select_department);
				MYSQL_RES *dep = mysql_store_result(mysql);
				MYSQL_ROW d = mysql_fetch_row(dep);

				//printf("%s\t%s\n", d[1], d[2]);

				int i_budget;
				sscanf(d[1], "%d", &i_budget);

				int i_counter;
				sscanf(d[2], "%d", &i_counter);

				if (i_budget > i_counter) {
					char inc_counter[200] = {0};
					strcat(inc_counter, "UPDATE departments SET counter = counter + 1 WHERE id='");
					strcat(inc_counter, row[3]);
					strcat(inc_counter, "'");

					//printf("%s\n\n", inc_counter); 

					mysql_query(mysql, inc_counter);

					char set_dep[200] = {0};
					strcat(set_dep, "UPDATE candidates SET department='");
					strcat(set_dep, row[3]);
					strcat(set_dep, "' WHERE id='");
					strcat(set_dep, row[0]);
					strcat(set_dep, "'");

					//printf("%s\n\n", inc_counter); 

					mysql_query(mysql, set_dep);


					printf("%s\t%s\t%s\t%s\n", row[0], row[1], row[2], row[3]);
				}

		}


	}

	mysql_free_result(result);

  mysql_close(mysql);
}
