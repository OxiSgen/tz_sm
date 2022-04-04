CREATE TABLE tb3 (
  id integer PRIMARY KEY AUTOINCREMENT,
  value varchar(100)
)

INSERT INTO tb3 (value)
SELECT t1.value FROM tb1 t1 LEFT JOIN tb2 t2 ON t1.value = t2.value
UNION
SELECT t2.value FROM tb2 t2 LEFT JOIN tb1 t1 ON t1.value = t2.value; 
