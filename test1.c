#include <sys/types.h>
#include <stdio.h>
#include <db.h>
#include <stdlib.h>
#include <string.h>

#define	DATABASE "access.db"

int main()
{
	DB *dbp;
	DBT key, data;
	int ret, t_ret;
	
	typedef struct PerInf {
	char name[10];
	char contact[15];
	char DoB[10];
	} PerInf;

	PerInf perinf;

	if ((ret = db_create(&dbp, NULL, 0)) != 0) {
		fprintf(stderr, "db_create: %s\n", db_strerror(ret));
		exit (1);
	}
	if ((ret = dbp->open(dbp,
	    NULL, DATABASE, NULL, DB_BTREE, DB_CREATE, 0664)) != 0) {
		dbp->err(dbp, ret, "%s", DATABASE);
		goto err;
	}

	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));

	strncpy(perinf.name, "Kai", 9);
	strncpy(perinf.contact, "0402332176", 14);
	strncpy(perinf.DoB, "19960829", 9);

	key.data = "Wu";
	key.size = sizeof("Wu");
	data.data = &perinf;
	data.size = sizeof(PerInf);

	if ((ret = dbp->put(dbp, NULL, &key, &data, 0)) == 0)
		printf("db: %s: key stored.\n", (char *)key.data);
	else {
		dbp->err(dbp, ret, "DB->put");
		goto err;
	}
	if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0) {
		PerInf *per = data.data;
		printf("db: Last name: %s, First name: %s, Contact: %s, DOB: %s\n",
		    (char *)key.data, per->name, per->contact, per->DoB);
	} else {
		dbp->err(dbp, ret, "DB->get");
		goto err;
	}

	if ((ret = dbp->del(dbp, NULL, &key, 0)) == 0)
		printf("db: %s: key was deleted.\n", (char *)key.data);
	else {
		dbp->err(dbp, ret, "DB->del");
		goto err;
	}

	if ((ret = dbp->get(dbp, NULL, &key, &data, 0)) == 0)
		printf("db: %s: key retrieved: data was %s.\n",
		    (char *)key.data, (char *)data.data);
	else
		dbp->err(dbp, ret, "DB->get");
err:	if ((t_ret = dbp->close(dbp, 0)) != 0 && ret == 0)
		ret = t_ret; 

	exit(ret);

}
