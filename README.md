# GrandOS

La idea todavía no está muy definida del todo, pero basicamente quiero desarrollar una herramienta para que junto a Debian, puedas montarte un servidor para tu casa de forma sencilla.

Toda la interfaz estará basada en web y cuando esté en una versión más desarrollada crear una iso premontada para facilitar su uso y que no sea necesario instalar Debian y varios pasos más para que funcione.

Actualmente la idea es que la interfaz web sea intuitiva, es decir, más parecida a un entorno de escritorio normal, pero dejando que si alguien sabe pueda tocar más profundamente y mediante terminal.

El objetivo principal sería que el servidor tenga buenas herramientas de ciberseguridad por defecto. Por ahora estoyinvestigando como hacer para centralizar el manejo de usuarios. El plan es que los contenedores puedan usar los usuarios del Host, para unificar los usuarios y no sean independientes e implementar una sección donde puedas manajar mediante RBAC (roles based access control) donde los grupos de linux serían los roles (obviamente no se va a incluir los grupos del sistema, solo los de los usuarios creados manualmente) para indicar a que contenedores o carpetas puede acceder o no. [Tovavía se está investigando como hacerlo, puede que se modifique]

