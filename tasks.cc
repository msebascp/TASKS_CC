#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

const int YEAR_MAX = 2100;  //Año máximo que se puede introducir en la fecha

const int YEAR_MIN = 2000;  //Año mínimo que se puede introducir en la fecha

const int KMAXNAME = 20;

const int KMAXDESC = 40;

struct Date{
  int day;
  int month;
  int year;
};

struct Task{
  string name;
  Date deadline;
  bool isDone;
  int time;
};

struct List{
  string name;
  vector<Task> tasks;
};

struct Project{
  int id;
  string name;
  string description;
  vector<List> lists;
};

struct ToDo{
  int nextId;
  string name;
  vector<Project> projects;
};

struct BinTask{
  char name[KMAXNAME];
  Date deadline;
  bool isDone;
  int time;
};

struct BinList{
  char name[KMAXNAME];
  unsigned numTasks;
};

struct BinProject{
  char name[KMAXNAME];
  char description[KMAXDESC];
  unsigned numLists;
};

struct BinToDo{
  char name[KMAXNAME];
  unsigned numProjects;
};

enum Error{
  ERR_OPTION,
  ERR_EMPTY,
  ERR_LIST_NAME,
  ERR_TASK_NAME,
  ERR_DATE,
  ERR_TIME,
  ERR_ID,
  ERR_PROJECT_NAME,
  ERR_FILE,
  ERR_ARGS
};

void error(Error e){
  switch(e){
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_EMPTY:
      cout << "ERROR: empty string" << endl;
      break;
    case ERR_LIST_NAME:
      cout << "ERROR: wrong list name" << endl;
      break;
    case ERR_TASK_NAME:
      cout << "ERROR: wrong task name" << endl;
      break;
    case ERR_DATE:
      cout << "ERROR: wrong date" << endl;
      break;
    case ERR_TIME:
      cout << "ERROR: wrong expected time" << endl;
      break;
    case ERR_ID:
      cout << "ERROR: wrong project id" << endl;
      break;
    case ERR_PROJECT_NAME:
      cout << "ERROR: wrong project name" << endl;
      break;
    case ERR_FILE:
      cout << "ERROR: cannot open file" << endl;
      break;
    case ERR_ARGS:
      cout << "ERROR: wrong arguments" << endl;
  }
}

/*Función que comprueba si un string esta vacío("") y muestra el error ERR_EMPTY si sí lo está
Parámetros: una cadena de carácteres string
Retorno: una variable boolena que es igual a false si el string no está vacío y true si sí lo está
*/
bool comprobarERR_EMPTY(string text){ 
  bool vacio = false;
  if(text == ""){
    error(ERR_EMPTY);
    vacio = true;
  }
  return(vacio);
}

/*Función que pide introducir el nombre de una lista, y que lo sigue pidiendo si se introduce un string vacío("")
Retorno: el nombre, de una lista, introducido
*/
string introducirNombre_lista(){  
  string nombre;
  do{  //en caso de introducir un string vacío vuelve a pedir un nombre para la lista
    cout << "Enter list name: ";
    getline(cin, nombre);
  }while(comprobarERR_EMPTY(nombre));
  return(nombre);
}

/*Función que pide introducir el nombre de una tarea
Retorno: el nombre, de una tarea, introducido
*/
string introducirNombre_task(){
  string nombre;
  cout << "Enter task name: ";
  getline(cin, nombre);
  return(nombre);
}

/*Función que comprueba si el nombre de una lista ya existe en el proyecto toDoList
Parámetros: el proyecto toDoList y el nombre introducido de una lista
Retorno: una variable boolena que es igual a false si la lista no se ha encontrado y true si sí se ha encontrado
*/
bool buscarLista(Project toDoList, string nombre){
  unsigned int i = 0;
  bool encontrado = false;
  while(encontrado == false && i < toDoList.lists.size()){  //Recorre el vector lists hasta encontrar la lista o hasta la última posición
    if(toDoList.lists[i].name == nombre){
      encontrado = true;
    }
    else{  
      i++;
    }
  }
  return(encontrado);
}

/*Función que pide introducir el nombre de una lista y comprueba si ya existe o no
Parámetros: el proyecto toDoList y el íncide del vector lists que servirá para saber su posición en caso de que exista
Retorno: una variable boolena que es igual a false si la lista no se ha encontrado y true si sí se ha encontrado
*/
bool pedirLista(Project toDoList, unsigned int &i){
  bool encontrado = false;
  string nombre = introducirNombre_lista();
  while(encontrado == false && i < toDoList.lists.size()){
    if(toDoList.lists[i].name == nombre){
      encontrado = true;
    }
    else{
      i++;
    }
  }
  return(encontrado);
}

/*Función que comprueba si una fecha introducida es correcta 
Parámetros: una fecha introducida en addTask
Retorno: una variable boolena que es igual a false si la fecha no es correcta y true si sí lo es
*/
bool comprobarFecha(Date f){
  bool correcto = true;
  int day = f.day;
  int month = f.month;
  int year = f.year;
  if(year < YEAR_MIN || year > YEAR_MAX){
    correcto = false;
  }
  else{
    switch(month){
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12: if(day > 31){  //Comprueba si la fecha es correcta para los meses 1, 3, 5, 7, 8, 10, 12
        correcto = false;
      }
      break;
      case 2: if(year % 100 == 0){  //Comprueba si la fecha es correcta para el mes de febrero
        if(year % 4 == 0 && year % 400 == 0){
          if(day > 29){
            correcto = false;
          }
        }
        else{
          if(day > 28){
            correcto = false;
          }
        }
      }
      else if(year % 4 == 0){
        if(day > 29){
          correcto = false;
        }
      }
      else{
        if(day > 28){
          correcto = false;
        }
      }
      break;
      case 4: 
      case 6:
      case 9:
      case 11: if(day > 30){  //Comprueba si la fecha es correcta para los meses 4, 6, 9, 11
        correcto = false;
      }
      break;
    }
  }
  return(correcto);
}

/*Función que muestra la información de una tarea en la función 7 Report y cambia el valor de las variables tiempo* y tarea*
Parámetros: el proyecto toDoList, el íncide del vector lists, el índice del vector tasks, 
el tiempo total de las tareas pendientes o realizadas,
y el número de tareas pendientes o realizadas
*/
void mostrarInfo_task(const Project toDoList, unsigned int i,
                      unsigned int j, int &tiempo, int &n_tareas){    
  if(toDoList.lists[i].tasks[j].isDone == false){                     
    cout << "[ ] ";
  }
  else{
    cout << "[X] ";
  }
  cout << "("  << toDoList.lists[i].tasks[j].time << ") ";
  cout << toDoList.lists[i].tasks[j].deadline.year << "-" << 
          toDoList.lists[i].tasks[j].deadline.month << "-" << 
          toDoList.lists[i].tasks[j].deadline.day;
  cout << " : " << toDoList.lists[i].tasks[j].name << endl;
  tiempo = tiempo + toDoList.lists[i].tasks[j].time;
  n_tareas++;
}

/*Función que comprueba si los nombres de dos tareas son iguales
Parámetros: el proyecto toDoList, el nombre introducido de una tarea, el íncide del vector lists, y el índice del vector tasks
Retorno: una variable boolena que es igual a false si los nombres no son iguales y true si sí lo son
*/
bool compararNombre_tasks(Project toDoList, string nombre, unsigned int i, unsigned int &j){ 
  bool iguales = false;
  while(iguales == false && j < toDoList.lists[i].tasks.size()){ 
    if(toDoList.lists[i].tasks[j].name == nombre){
      iguales = true;
    }
    else{
      j++;
    }
  }  
  return(iguales);
}

void showMainMenu(){
  cout << "1- Edit project" << endl
       << "2- Add list" << endl
       << "3- Delete list" << endl 
       << "4- Add task" << endl
       << "5- Delete task" << endl
       << "6- Toggle task" << endl
       << "7- Report" << endl
       << "b- Back to main menu" << endl
       << "Option: ";
}

void editProject(Project &toDoList){
  do{ //Pedirá introducir un nombre mientras el nombre esté vació("")
    cout << "Enter project name: ";
    getline(cin, toDoList.name);
  }while(comprobarERR_EMPTY(toDoList.name));
  cout << "Enter project description: ";
  getline(cin, toDoList.description);
}

void addList(Project &toDoList){
  string nombre;
  nombre = introducirNombre_lista();
  if(buscarLista(toDoList, nombre)){
    error(ERR_LIST_NAME);
  }
  else{
    List Lista;
    Lista.name = nombre;
    toDoList.lists.push_back(Lista);
  }
}

void deleteList(Project &toDoList){
  unsigned int i = 0; //índice que recorre el vector lists
  if(pedirLista(toDoList, i)){  //si la lista introducida existe, se elimina
    toDoList.lists.erase(toDoList.lists.begin() + i);
  }
  else{
    error(ERR_LIST_NAME);
  }
}

void addTask(Project &toDoList){
  unsigned int i = 0; //índice que recorre el vector lists
  if(pedirLista(toDoList, i)){ //Si la lista introducida existe se ejecutarán las sentencias, si no mostrará error
    Task Tarea; //Servirá para añadir los datos de la tarea al vector tasks
    Tarea.name = introducirNombre_task();
    cout << "Enter deadline: ";
    cin >> Tarea.deadline.day; 
    cin.get();
    cin >> Tarea.deadline.month;
    cin.get();
    cin >> Tarea.deadline.year;
    cin.get();
    if(comprobarFecha(Tarea.deadline)){ //Si la fecha introducida es correcta las sentencias se ejecutarán, si no mostrará error
      Tarea.isDone = false;
      cout << "Enter expected time: ";     
      cin >> Tarea.time;
      if(Tarea.time > 0 && Tarea.time < 181){  //Si el tiempo esperado introducido es correcto finalmente 
        toDoList.lists[i].tasks.push_back(Tarea); //  se añadirá la tarea al vector tasks,si no mostrará error
      }
      else{
        error(ERR_TIME);
      }
    }
    else{
      error(ERR_DATE);
    }  
  }
  else{
    error(ERR_LIST_NAME);
  }
}

void deleteTask(Project &toDoList){
  unsigned int i = 0; //índice que recorre el vector lists
  unsigned int j = 0; //índice que recorre el vector tasks
  bool task_encontrada = false;
  if(pedirLista(toDoList, i)){ //Si la lista introducida existe se ejecutarán las sentencias, si no mostrará error
    string nombre = introducirNombre_task();
    while(j < toDoList.lists[i].tasks.size()){ //se recorre el vector tasks
      if(compararNombre_tasks(toDoList, nombre, i, j)){ //Se revisa si la tarea introducida existe,si existe se elimina
        toDoList.lists[i].tasks.erase(toDoList.lists[i].tasks.begin() + j);
        task_encontrada = true;
        j--;
      }
      j++;
    }
    if(task_encontrada == false){ //Si no se ha encontrado la tarea se mostrará error
      error(ERR_TASK_NAME);
    }
  }
  else{
    error(ERR_LIST_NAME);
  }
}

void toggleTask(Project &toDoList){
  unsigned int i = 0; //índice que recorre el vector lists
  unsigned int j = 0; //índice que recorre el vector tasks
  bool task_encontrada = false;
  if(pedirLista(toDoList, i)){  //Si la lista introducida existe se ejecutarán las sentencias, si no mostrará error
    string nombre = introducirNombre_task();
    while(j < toDoList.lists[i].tasks.size()){  //Se recorre el vector tasks          
      if(compararNombre_tasks(toDoList, nombre, i, j)){ //Se revisa si la tarea introducida existe, si existe se cambia su estado(isDone)
        if(toDoList.lists[i].tasks[j].isDone){              
          toDoList.lists[i].tasks[j].isDone = false;
        }
        else{
          toDoList.lists[i].tasks[j].isDone = true;
        }
        task_encontrada = true;
      }
      j++;
    }
    if(task_encontrada == false){ //Si la task introducida no existe se mostrará error
      error(ERR_TASK_NAME);
    }
  }
  else{
    error(ERR_LIST_NAME);
  }
}

void report(const Project &toDoList){
  unsigned int i, j; //índices que recorren el vector lists y el vector tasks, respectivamente 
  unsigned int i_aux, j_aux; //índices que indican la posición de la tarea de mayor prioridad
  int tiempoLeft = 0; //minutos totales de las tareas pendientes y realizadas
  int tiempoDone = 0; 
  int tareaLeft = 0; //cantidad de tareas finalizadas y pendientes
  int tareaDone = 0; 
  Date fecha; //Fecha(deadline) de la tarea de mayor prioridad
  cout << "Name: " << toDoList.name << endl;
  if(toDoList.description != ""){ //Si la descripción está vacía no se mostrará el mensaje "Description: "
    cout << "Description: " << toDoList.description << endl;
  }
  for(i = 0; i < toDoList.lists.size(); i++){ //se recorre el vector lists
    cout << toDoList.lists[i].name << endl;
    for(j = 0; j < toDoList.lists[i].tasks.size(); j++){ //en este for se mostrarán los datos de las tareas pendientes
      if(toDoList.lists[i].tasks[j].isDone == false){
        mostrarInfo_task(toDoList, i, j, tiempoLeft, tareaLeft);
        if(tareaLeft == 1){ //Con los siguientes if else obtenemos los datos de la tarea pendiente con mayor prioridad
          fecha = toDoList.lists[i].tasks[j].deadline;
          i_aux = i;
          j_aux = j;
        }
        else if(toDoList.lists[i].tasks[j].deadline.year < fecha.year){
          fecha = toDoList.lists[i].tasks[j].deadline;
          i_aux = i;
          j_aux = j;
        }
        else if(toDoList.lists[i].tasks[j].deadline.month < fecha.month &&
                toDoList.lists[i].tasks[j].deadline.year == fecha.year){
          fecha = toDoList.lists[i].tasks[j].deadline;
          i_aux = i;
          j_aux = j;
        }
        else if(toDoList.lists[i].tasks[j].deadline.day < fecha.day &&
                toDoList.lists[i].tasks[j].deadline.month == fecha.month &&
                toDoList.lists[i].tasks[j].deadline.year == fecha.year){
          fecha = toDoList.lists[i].tasks[j].deadline;
          i_aux = i;
          j_aux = j;
        }
      }
    }
    for(j = 0; j < toDoList.lists[i].tasks.size(); j++){ //en este for se mostrarán los datos de las tareas realizadas
      if(toDoList.lists[i].tasks[j].isDone == true){
        mostrarInfo_task(toDoList, i, j, tiempoDone, tareaDone);
      }
    }
  }
  cout << "Total left: " << tareaLeft << " (" << tiempoLeft << " minutes)" << endl;
  cout << "Total done: " << tareaDone << " (" << tiempoDone << " minutes)" << endl;
  if(tareaLeft > 0){  //si hay alguna tarea pendiente se mostrarán los datos de la de mayor prioridad
    cout << "Highest priority: " << toDoList.lists[i_aux].tasks[j_aux].name << " (" << 
            fecha.year << "-" << fecha.month << "-" << fecha.day << ")" << endl;  
  }
}

/*Función que pide introducir el id de un proyecto y comprueba si existe
Parámetros: la variable toDoList que gestiona los proyectos, y el íncide del vector projects que servirá para conocer la posición del projecto
en caso de que exista
Retorno: una variable boolena que es igual a true si el proyecto existe y false si no
*/
bool buscarIdProject(ToDo toDoList, unsigned int &i){
  bool encontrado = false;
  int id;
  cout << "Enter project id: ";
  cin >> id;
  cin.get();
  while(encontrado == false && i < toDoList.projects.size()){
    if(toDoList.projects[i].id == id){
      encontrado = true;
    }
    else{
      i++;
    }
  }
  return(encontrado);
}

/*Función que pide introducir el nombre de un proyecto 
Retorno: el nombre introducido
*/
string introducirNombre_project(){
  string nombre;
  do{
    cout << "Enter project name: ";
    getline(cin, nombre);
  }while(comprobarERR_EMPTY(nombre));
  return(nombre);
}

/*Función que comprueba si existe un proyecto
Parámetros: la variable toDoList que gestiona los proyectos, y el nombre de un proyecto
Retorno: una variable boolena que es igual a true si el proyecto existe y false si no
*/
bool buscarNombreProject(ToDo toDoList, string nombre){
  bool encontrado = false;
  unsigned int i = 0;
  while(encontrado == false && i < toDoList.projects.size()){
    if(toDoList.projects[i].name == nombre){
      encontrado = true;
    }
    else{
      i++;
    }
  }
  return(encontrado);
}

/*Función que pide introducir el nombre de un fichero
Retorno: el nombre de un fichero
*/
string introducirNombreFichero(){
  string nombre;
  cout << "Enter filename: ";
  getline(cin, nombre);
  return(nombre);
}

/*Función que importa los datos de un fichero de texto y complementa a la función importProjects
Parámetros: la variable toDoList que gestiona los proyectos, nextId que corresponde al id que se asignará al próximo proyecto introducido, 
y un fichero
*/
void importAux(ToDo &toDoList, int &nextId, ifstream &fichero){
  Project project;  //
  List lista;       //variables de tipo project, list, y task que permite almacenar los datos leídos en toDoList
  Task tarea;       //
  bool pcorrecto; //se mantiene a true si los datos leídos son correctos
  char caracter;  //variable con la que leeremos carácteres del fichero
  string texto; //variable con la que leeremos cadenas de texto del fichero
  while(!fichero.eof()){
    pcorrecto = true;
    fichero >> caracter;
    if(caracter == '<'){
      while(caracter != '>'){
        if(caracter != '@' || !pcorrecto){  //si caracter == '@' significa que primeor hay que leer los datos de una lista 
          fichero >> caracter;              //se leerá un carácter si el proyecto no es correcto o caracter != '@'
        }
        if(caracter == '#'){  //si se lee '#' a continuación se leerá el nombre de un proyecto
          getline(fichero, texto);
          if(buscarNombreProject(toDoList, texto)){ //si ese nombre ya está en uso cambia el valor de pcorrecto
            pcorrecto = false;
          }
          else{
            project.name = texto;
            project.description = "";
          }
        }
        if(caracter == '*' && pcorrecto){ //si se lee '*' a continuación se leerá la descripción de un proyecto y sus listas
          getline(fichero, texto);
          project.description = texto;
        }
        if(caracter == '@' && pcorrecto){ //si se lee '@' a continuación se leerá el nombre de una lista y sus tareas 
          getline(fichero, texto);
          lista.name = texto;
          do{
            fichero >> caracter;
            if(caracter != '@' && caracter != '\n' && caracter != '>'){ //si hay, se leen tareas
              if(caracter == '|'){  //si se lee '|' significa que la tarea no tiene nombre
                tarea.name = "";
              }
              else{ //se lee el nombre de la tarea
                getline(fichero, texto,'|');
                texto = caracter + texto;
                tarea.name = texto;
              }
              getline(fichero, texto, '/'); //leemos el día de deadline
              tarea.deadline.day = stoi(texto);
              getline(fichero, texto, '/'); //leemos el mes
              tarea.deadline.month = stoi(texto);
              getline(fichero, texto, '|'); //leemos el año
              tarea.deadline.year = stoi(texto);
              if(comprobarFecha(tarea.deadline)){ //si la fecha es correcta continuamos leyendo los datos
                getline(fichero, texto, '|'); //leemos si la tarea está hecha o no
                if(texto == "T"){
                  tarea.isDone = true;
                }
                else if(texto == "F"){
                  tarea.isDone = false;
                }
                fichero >> texto; //leemos el tiempo estimado para la tarea
                tarea.time = stoi(texto);
                if(tarea.time > 0 && tarea.time < 181){ //si la el tiempo es correcto se añade la tarea a la lista
                  lista.tasks.push_back(tarea);
                }
                else{
                  error(ERR_TIME);
                }
              }
              else{
                error(ERR_DATE);
              }
            }
          }while(caracter != '@' && caracter != '>'); //mientras siga habiendo tareas se leerán
          project.lists.push_back(lista); //añadimos la lista leída a project
          lista.tasks.clear(); //vacíamos la variable lista para poder leer la próxima lista
        }
        if(caracter == '>' && pcorrecto){ //si se lee '>' y los datos del proyecto leído son correctos, este se añade a toDoList
          project.id = nextId;
          nextId++;
          toDoList.projects.push_back(project);
          project.lists.clear();  //vacíamos la variable project para poder leer el próximo proyecto
        }
      }
    }
  }
}

/*Función que carga los datos de un fichero binario y complementa a la función loadProjects
Parámetros: la variable toDoList que gestiona los proyectos, nextId que corresponde al id que se asignará al próximo proyecto introducido, 
y un fichero
*/
void loadAux(ToDo &toDoList, int &nextId, ifstream &fichero){
  toDoList.projects.clear();
  nextId = 1;
  unsigned int i;
  unsigned int j;
  unsigned int k;
  BinToDo btoDo;
  BinProject bproject;
  BinList blist;
  BinTask btask;
  Project project;
  List list;
  Task task;
  fichero.read((char *)&btoDo, sizeof(BinToDo));
  toDoList.name = btoDo.name;
  for(i = 0; i < btoDo.numProjects; i++){
    fichero.read((char *)&bproject, sizeof(BinProject));
    project.id = nextId;
    nextId++;
    project.name = bproject.name;
    project.description = bproject.description;
    for(j = 0; j < bproject.numLists; j++){
      fichero.read((char *)&blist, sizeof(BinList));
      list.name = blist.name;
      for(k = 0; k < blist.numTasks; k++){
        fichero.read((char *)&btask, sizeof(BinTask));
        task.name = btask.name;
        task.deadline = btask.deadline;
        task.isDone = btask.isDone;
        task.time = btask.time;
        list.tasks.push_back(task);
      }
      project.lists.push_back(list);
      list.tasks.clear();
    }
    toDoList.projects.push_back(project);
    project.lists.clear();
  }
}

/*Función que exporta los datos de todos los proyectos en un fichero, y que complementa a la función exportProjects
Parámetros: la variable toDoList que gestiona los proyectos
*/
void exportAllAux(ToDo &toDoList){
  unsigned int i; //índice que recorre el vector projects
  unsigned int j; //índice que recorre el vector lists
  unsigned int k; //índice que recorre el vector tasks
  string nombreFichero = introducirNombreFichero();
  ofstream fichero(nombreFichero);
  if(fichero.is_open()){
    for(i = 0; i < toDoList.projects.size();i++){ //recorre el vector projects y muestra su información
      fichero << "<" << endl
              << "#" << toDoList.projects[i].name << endl;
      if(toDoList.projects[i].description != ""){
        fichero << "*" << toDoList.projects[i].description << endl;
      }
      for(j = 0; j < toDoList.projects[i].lists.size(); j++){ //recorre el vector lists y muestra su información
        fichero << "@" << toDoList.projects[i].lists[j].name << endl;
        for(k = 0; k < toDoList.projects[i].lists[j].tasks.size(); k++){  //recorre el vector tasks y muestra su información
          fichero << toDoList.projects[i].lists[j].tasks[k].name << "|"
                  << toDoList.projects[i].lists[j].tasks[k].deadline.day << "/"
                  << toDoList.projects[i].lists[j].tasks[k].deadline.month << "/"
                  << toDoList.projects[i].lists[j].tasks[k].deadline.year << "|";
          if(toDoList.projects[i].lists[j].tasks[k].isDone){
            fichero << "T|";
          }
          else{
            fichero << "F|";
          }
          fichero << toDoList.projects[i].lists[j].tasks[k].time << endl;
        }
      }
      fichero << ">" << endl;
    }
    fichero.close();
  }
  else{
    error(ERR_FILE);
  }
}

/*Función que exporta los datos de un proyecto en concreto en un fichero, y que complementa a la función exportProjects
Parámetros: la variable toDoList que gestiona los proyectos
*/
void exportAux(ToDo &toDoList){
  unsigned int i = 0;
  unsigned int j;
  unsigned int k;
  if(buscarIdProject(toDoList, i)){
    string nombreFichero = introducirNombreFichero();
    ofstream fichero(nombreFichero);
    if(fichero.is_open()){     
      fichero << "<" << endl
              << "#" << toDoList.projects[i].name << endl;
      if(toDoList.projects[i].description != ""){
        fichero << "*" << toDoList.projects[i].description << endl;
      }
      for(j = 0; j < toDoList.projects[i].lists.size(); j++){
        fichero << "@" << toDoList.projects[i].lists[j].name << endl;
        for(k = 0; k < toDoList.projects[i].lists[j].tasks.size(); k++){
          fichero << toDoList.projects[i].lists[j].tasks[k].name << "|"
                  << toDoList.projects[i].lists[j].tasks[k].deadline.day << "/"
                  << toDoList.projects[i].lists[j].tasks[k].deadline.month << "/"
                  << toDoList.projects[i].lists[j].tasks[k].deadline.year << "|";
          if(toDoList.projects[i].lists[j].tasks[k].isDone){
            fichero << "T|";
          }
          else{
            fichero << "F|";
          }
          fichero << toDoList.projects[i].lists[j].tasks[k].time << endl;
        }
      }
      fichero << ">" << endl;
    }
    fichero.close();
  }
  else{
    error(ERR_ID);
  }
}

void showMainMenu2(){
  cout << "1- Project menu" << endl
       << "2- Add project" << endl
       << "3- Delete project" << endl 
       << "4- Import projects" << endl
       << "5- Export projects" << endl
       << "6- Load data" << endl
       << "7- Save data" << endl
       << "8- Summary" << endl
       << "q- Quit" << endl
       << "Option: ";
}

void projectMenu(ToDo &toDoList){
  char option;
  unsigned int i = 0; //índice que recorre el vector projects
  if(buscarIdProject(toDoList, i)){
    do{
      showMainMenu();
      cin >> option;
      cin.get();
      switch(option){
        case '1': editProject(toDoList.projects[i]);
                  break;
        case '2': addList(toDoList.projects[i]);
                  break;
        case '3': deleteList(toDoList.projects[i]);
                  break;
        case '4': addTask(toDoList.projects[i]);
                  break;
        case '5': deleteTask(toDoList.projects[i]);
                  break;
        case '6': toggleTask(toDoList.projects[i]);
                  break;
        case '7': report(toDoList.projects[i]);
                  break;
        case 'b': break;
        default: error(ERR_OPTION);
      }
    }while(option!='b');
  }
  else{
    error(ERR_ID);
  }
}

void addProject(ToDo &toDoList, int &nextId){
  string nombre = introducirNombre_project();
  if(buscarNombreProject(toDoList, nombre)){
    error(ERR_PROJECT_NAME);  //si el nombre de proyecto ya existe mostrará error
  }
  else{
    Project proyecto; //proyecto que se añadirá al vector projects
    proyecto.name = nombre;
    cout << "Enter project description: ";
    getline(cin, proyecto.description);
    proyecto.id = nextId;
    toDoList.projects.push_back(proyecto);
    nextId++;
  }
}

void deleteProject(ToDo &toDoList){
  unsigned int i = 0; //índice que recorre el vector projects
  if(buscarIdProject(toDoList, i)){ //si la id introducida existe se elimina el proyecto correspondiente
    toDoList.projects.erase(toDoList.projects.begin() + i);
  }
  else{
    error(ERR_ID);
  }
}

void importProjects(ToDo &toDoList, int &nextId){ 
  string nombreFichero = introducirNombreFichero();
  ifstream fichero(nombreFichero);
  if(fichero.is_open()){  //si el fichero se ha abierto correctamente importamos los datos 
    importAux(toDoList, nextId, fichero); //funcion con la importamos los datos
    fichero.close();
  }
  else{
    error(ERR_FILE);
  }
}

void exportProjects(ToDo &toDoList){
  char respuesta;
  do{
    cout << "Save all projects [Y/N]?: ";
    cin >> respuesta;
    cin.get();
  }while(respuesta != 'Y' && respuesta != 'y' && respuesta != 'N' && respuesta != 'n');
  if(respuesta == 'Y' || respuesta == 'y'){
    exportAllAux(toDoList);
  }
  else if(respuesta == 'N' || respuesta == 'n'){
    exportAux(toDoList);
  }
}

void loadData(ToDo &toDoList, int &nextId){
  char respuesta;
  string nombreFichero = introducirNombreFichero();
  ifstream fichero(nombreFichero, ios::binary);  
  if(fichero.is_open()){
    do{
      cout << "Confirm [Y/N]?: ";
      cin >> respuesta;
      cin.get();
    }while(respuesta != 'Y' && respuesta != 'y' && respuesta != 'N' && respuesta != 'n');
    if(respuesta == 'Y' || respuesta == 'y'){
      loadAux(toDoList, nextId, fichero);
      fichero.close();
    }
    else{
      fichero.close();
    }
  }
  else{
    error(ERR_FILE);
  }
}

void saveData(ToDo &toDoList){
  unsigned int i;
  unsigned int j;
  unsigned int k;
  string nombreFichero = introducirNombreFichero();
  BinToDo btoDo;
  BinProject bproject;
  BinList blist;
  BinTask btask;
  ofstream fichero(nombreFichero, ios::binary);
  if(fichero.is_open()){
    strncpy(btoDo.name, toDoList.name.c_str(), KMAXNAME - 1);
    btoDo.name[KMAXNAME - 1] = '\0';
    btoDo.numProjects = toDoList.projects.size();
    fichero.write((const char *)&btoDo, sizeof(BinToDo));
    for(i = 0; i < toDoList.projects.size(); i++){
      strncpy(bproject.name, toDoList.projects[i].name.c_str(), KMAXNAME - 1);
      bproject.name[KMAXNAME - 1] = '\0';
      strncpy(bproject.description, toDoList.projects[i].description.c_str(), KMAXDESC - 1);
      bproject.description[KMAXDESC - 1] = '\0';
      bproject.numLists = toDoList.projects[i].lists.size();
      fichero.write((const char *)&bproject, sizeof(BinProject));
      for(j = 0; j < toDoList.projects[i].lists.size(); j++){
        strncpy(blist.name, toDoList.projects[i].lists[j].name.c_str(), KMAXNAME - 1);
        blist.name[KMAXNAME - 1] = '\0';
        blist.numTasks = toDoList.projects[i].lists[j].tasks.size();
        fichero.write((const char *)&blist, sizeof(BinList));
        for(k = 0; k < toDoList.projects[i].lists[j].tasks.size(); k++){
          strncpy(btask.name, toDoList.projects[i].lists[j].tasks[k].name.c_str(), KMAXNAME - 1);
          btask.name[KMAXNAME - 1] = '\0';
          btask.deadline = toDoList.projects[i].lists[j].tasks[k].deadline;
          btask.isDone = toDoList.projects[i].lists[j].tasks[k].isDone;
          btask.time = toDoList.projects[i].lists[j].tasks[k].time;
          fichero.write((const char *)&btask, sizeof(BinTask));
        }
      }
    }
    fichero.close();
  }
  else{
    error(ERR_FILE);
  }
}

void summary(ToDo &toDoList){
  unsigned int i;
  unsigned int j;
  unsigned int k;
  for(i = 0; i < toDoList.projects.size(); i++){
    int nTareas = 0; 
    int nHechas = 0;
    for(j = 0; j < toDoList.projects[i].lists.size(); j++){
      for(k = 0; k < toDoList.projects[i].lists[j].tasks.size(); k++){
        nTareas++;
        if(toDoList.projects[i].lists[j].tasks[k].isDone){
          nHechas++;
        }
      }
    }
    cout << "(" << toDoList.projects[i].id << ") "
         << toDoList.projects[i].name 
         << "[" << nHechas << "/" << nTareas << "]" << endl;
  }  
}

int main(int argc, char *argv[]){
  ToDo toDoList;
  toDoList.nextId = 1;
  toDoList.name = "My ToDo list";
  bool correcto = true;
  if(argc > 5 || argc == 2 || argc == 4){
    correcto = false;
    error(ERR_ARGS);
  }
  else if(argc == 3){
    if(argv[1][0] == '-' && argv[1][1] == 'i'){
      ifstream fichero(argv[2]);
      if(fichero.is_open()){
        importAux(toDoList, toDoList.nextId, fichero);
        fichero.close();
      }
      else{
        error(ERR_FILE);
        correcto = false;
      }
    }
    else if(argv[1][0] == '-' && argv[1][1] == 'l'){
      ifstream fichero(argv[2], ios::binary);
      if(fichero.is_open()){
        loadAux(toDoList, toDoList.nextId, fichero);
        fichero.close();
      }
      else{
        error(ERR_FILE);
        correcto = false;
      }
    }
    else{
      error(ERR_ARGS);
      correcto = false;
    }
  }
  else if(argc == 5){
    if(argv[1][0] == '-' && (argv[1][1] == 'i' || argv[1][1] == 'l') &&
      argv[3][0] == '-' && (argv[3][1] == 'i' || argv[3][1] == 'l') &&
      argv[1][1] != argv[3][1]){
        int i;
        for(i = 1; i < argc; i = i + 2){
          if(argv[i][1] == 'l'){
            ifstream fichero(argv[i + 1], ios::binary);
            if(fichero.is_open()){
              loadAux(toDoList, toDoList.nextId, fichero);
              fichero.close();
            }
            else{
              error(ERR_FILE);
              correcto = false;
            }
          }
        }
        for(i = 1; i < argc; i = i + 2){
          if(argv[i][1] == 'i'){
            ifstream fichero(argv[i +1]);
            if(fichero.is_open()){
              importAux(toDoList, toDoList.nextId, fichero);
              fichero.close();
            }
            else{
              error(ERR_FILE);
              correcto = false;
            }
          }
        }
      }
    else{
      error(ERR_ARGS);
      correcto = false;
    }  
  }
  if(correcto){
    char option;
    do{
      showMainMenu2();
      cin >> option;
      cin.get();
      switch(option){
        case '1': projectMenu(toDoList);
                  break;
        case '2': addProject(toDoList, toDoList.nextId);
                  break;
        case '3': deleteProject(toDoList);
                  break;
        case '4': importProjects(toDoList, toDoList.nextId);
                  break;
        case '5': exportProjects(toDoList);
                  break;
        case '6': loadData(toDoList, toDoList.nextId);
                  break;
        case '7': saveData(toDoList);
                  break;
        case '8': summary(toDoList);
                  break;
        case 'q': break;
        default: error(ERR_OPTION);
      }
    }while(option!='q');
  }
  return 0;    
}