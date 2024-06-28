#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // for std::find
#include <map>       // for std::map
#include <ctime>     // for std::time
#include <cstdlib>   // for std::rand, std::srand

using namespace std;

// Clase para libros
class Libro {
	private:
		string id;
		string titulo;
		string autor;
		int numCopias;
		int prestados;

	public:
		Libro(string _id, string _titulo, string _autor, int _numCopias): id(_id), titulo(_titulo), autor(_autor), numCopias(_numCopias), prestados(0) {}

		string getId() const{
			return id;
		}
		string getTitulo() const{
			return titulo;
		}
		string getAutor() const{
			return autor;
		}
		int getNumCopias() const{
			return numCopias;
		}
		int getPrestados() const{
			return prestados;
		}

		void prestarLibro() {
			if (numCopias > 0) {
				numCopias--;
				prestados++;
			} else {
				cout << "No hay copias disponibles para prestar." << endl;
			}
		}

		void devolverLibro() {
			if (prestados > 0) {
				numCopias++;
				prestados--;
			} else {
				cout << "No hay libros prestados para devolver." << endl;
			}
		}

		// Función para actualizar información del libro
		void actualizarLibro(const string& nuevoTitulo, const string& nuevoAutor, int nuevasCopias) {
			titulo = nuevoTitulo;
			autor = nuevoAutor;
			numCopias = nuevasCopias;
		}
};

// Clase para clientes
class Cliente {
	private:
		string id;
		string nombre;

	public:
		Cliente(string _id, string _nombre) : id(_id), nombre(_nombre) {}

		string getId() const {
			return id;
		}
		string getNombre() const {
			return nombre;
		}

		// Función para actualizar información del cliente
		void actualizarCliente(const string& nuevoNombre) {
			nombre = nuevoNombre;
		}
};

// Gestor de la biblioteca
class Biblioteca {
	private:
		vector<Libro> libros;
		vector<Cliente> clientes;
		map<string, vector<string>> prestamos; // Mapa para registrar préstamos de libros por cliente

	public:
		void agregarLibro(const Libro& libro) {
			libros.push_back(libro);
		}

		void mostrarLibros() const {
			for (const auto& libro : libros) {
				cout << "ID: " << libro.getId() << ", Título: " << libro.getTitulo() << ", Autor: " << libro.getAutor()
				     << ", Copias disponibles: " << libro.getNumCopias() << ", Prestados: " << libro.getPrestados() << endl;
			}
		}

		void prestarLibro(const string& idLibro, const string& idCliente) {
			auto itLibro = find_if(libros.begin(), libros.end(), [&idLibro](const Libro& libro) {
				return libro.getId() == idLibro;
			});

			auto itCliente = find_if(clientes.begin(), clientes.end(), [&idCliente](const Cliente& cliente) {
				return cliente.getId() == idCliente;
			});

			if (itLibro != libros.end() && itCliente != clientes.end()) {
				if (itLibro->getNumCopias() > 0) {
					itLibro->prestarLibro();
					prestamos[idCliente].push_back(idLibro); // Registrar préstamo
					cout << "Libro prestado correctamente a " << itCliente->getNombre() << "." << endl;
				} else {
					cout << "No hay copias disponibles para prestar." << endl;
				}
			} else {
				cout << "Libro o cliente no encontrado en la biblioteca." << endl;
			}
		}

		void devolverLibro(const string& idLibro, const string& idCliente) {
			auto itLibro = find_if(libros.begin(), libros.end(), [&idLibro](const Libro& libro) {
				return libro.getId() == idLibro;
			});

			auto itCliente = find_if(clientes.begin(), clientes.end(), [&idCliente](const Cliente& cliente) {
				return cliente.getId() == idCliente;
			});

			if (itLibro != libros.end() && itCliente != clientes.end()) {
				auto itPrestamo = find(prestamos[idCliente].begin(), prestamos[idCliente].end(), idLibro);
				if (itPrestamo != prestamos[idCliente].end()) {
					itLibro->devolverLibro();
					prestamos[idCliente].erase(itPrestamo); // Eliminar registro de préstamo
					cout << "Libro devuelto correctamente por " << itCliente->getNombre() << "." << endl;
				} else {
					cout << "El cliente no tiene prestado este libro." << endl;
				}
			} else {
				cout << "Libro o cliente no encontrado en la biblioteca." << endl;
			}
		}

		void actualizarLibro(const string& idLibro, const Libro& nuevoLibro) {
			auto it = find_if(libros.begin(), libros.end(), [&idLibro](const Libro& libro) {
				return libro.getId() == idLibro;
			});

			if (it != libros.end()) {
				*it = nuevoLibro;
				cout << "Libro actualizado correctamente." << endl;
			} else {
				cout << "Libro no encontrado en la biblioteca. No se pudo actualizar." << endl;
			}
		}

		void eliminarLibro(const string& idLibro) {
			auto it = find_if(libros.begin(), libros.end(), [&idLibro](const Libro& libro) {
				return libro.getId() == idLibro;
			});

			if (it != libros.end()) {
				libros.erase(it);
				cout << "Libro eliminado correctamente." << endl;
			} else {
				cout << "Libro no encontrado en la biblioteca. No se pudo eliminar." << endl;
			}
		}

		// Funciones para clientes
		void agregarCliente(const Cliente& cliente) {
			auto it = find_if(clientes.begin(), clientes.end(), [&cliente](const Cliente& c) {
				return c.getId() == cliente.getId();
			});

			if (it == clientes.end()) {
				clientes.push_back(cliente);
			} else {
				cout << "Cliente ya existe en el sistema." << endl;
			}
		}

		void mostrarClientes() const {
			for (const auto& cliente : clientes) {
				cout << "ID: " << cliente.getId() << ", Nombre: " << cliente.getNombre() << endl;
			}
		}

		// Función para actualizar cliente
		void actualizarCliente(const string& idCliente, const string& nuevoNombre) {
			auto it = find_if(clientes.begin(), clientes.end(), [&idCliente](const Cliente& cliente) {
				return cliente.getId() == idCliente;
			});

			if (it != clientes.end()) {
				it->actualizarCliente(nuevoNombre);
				cout << "Cliente actualizado correctamente." << endl;
			} else {
				cout << "Cliente no encontrado en la biblioteca. No se pudo actualizar." << endl;
			}
		}

		// Menú principal de la biblioteca
		void menuPrincipal() {
			int opcion;
			do {
				cout << "\n--- MENÚ PRINCIPAL ---" << endl;
				cout << "1. Gestión de libros" << endl;
				cout << "2. Gestión de clientes" << endl;
				cout << "3. Mostrar préstamos" << endl;
				cout << "4. Salir" << endl;
				cout << "Ingrese opción: ";
				cin >> opcion;

				switch (opcion) {
					case 1:
						menuLibros();
						break;
					case 2:
						menuClientes();
						break;
					case 3:
						mostrarPrestamos();
						break;
					case 4:
						cout << "Saliendo del programa." << endl;
						break;
					default:
						cout << "Opción inválida. Intente de nuevo." << endl;
						break;
				}
			} while (opcion != 4);
		}

		// Menú de gestión de libros
		void menuLibros() {
			int opcion;
			do {
				cout << "\n--- MENÚ DE GESTIÓN DE LIBROS ---" << endl;
				cout << "1. Agregar libro" << endl;
				cout << "2. Mostrar libros" << endl;
				cout << "3. Prestar libro" << endl;
				cout << "4. Devolver libro" << endl;
				cout << "5. Actualizar libro" << endl;
				cout << "6. Eliminar libro" << endl;
				cout << "7. Volver al menú principal" << endl;
				cout << "Ingrese opción: ";
				cin >> opcion;

				switch (opcion) {
					case 1:
						agregarLibroDesdeInput();
						break;
					case 2:
						mostrarLibros();
						break;
					case 3:
						prestarLibroPorCodigo();
						break;
					case 4:
						devolverLibroPorCodigo();
						break;
					case 5:
						actualizarLibroPorCodigo();
						break;
					case 6:
						eliminarLibroPorCodigo();
						break;
					case 7:
						cout << "Volviendo al menú principal." << endl;
						break;
					default:
						cout << "Opción inválida. Intente de nuevo." << endl;
						break;
				}
			} while (opcion != 7);
		}

		void agregarLibroDesdeInput() {
			string id, titulo, autor;
			int numCopias;

			cout << "Ingrese ID del libro: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, id);
			cout << "Ingrese título del libro: ";
			getline(cin, titulo);
			cout << "Ingrese autor del libro: ";
			getline(cin, autor);
			cout << "Ingrese número de copias disponibles: ";
			cin >> numCopias;

			Libro libro(id, titulo, autor, numCopias);
			agregarLibro(libro);

			cout << "Libro agregado correctamente." << endl;
		}

		void prestarLibroPorCodigo() {
			string idLibro, idCliente;
			cout << "Ingrese ID del libro a prestar: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, idLibro);
			cout << "Ingrese ID del cliente que realiza el préstamo: ";
			getline(cin, idCliente);

			prestarLibro(idLibro, idCliente);
		}

		void devolverLibroPorCodigo() {
			string idLibro, idCliente;
			cout << "Ingrese ID del libro a devolver: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, idLibro);
			cout << "Ingrese ID del cliente que devuelve el libro: ";
			getline(cin, idCliente);

			devolverLibro(idLibro, idCliente);
		}

		void actualizarLibroPorCodigo() {
			string id;
			cout << "Ingrese ID del libro a actualizar: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, id);

			// Obtener nuevos datos del libro
			string nuevoTitulo, nuevoAutor;
			int nuevasCopias;

			cout << "Ingrese nuevo título del libro: ";
			getline(cin, nuevoTitulo);
			cout << "Ingrese nuevo autor del libro: ";
			getline(cin, nuevoAutor);
			cout << "Ingrese nuevo número de copias disponibles: ";
			cin >> nuevasCopias;

			Libro nuevoLibro(id, nuevoTitulo, nuevoAutor, nuevasCopias);
			actualizarLibro(id, nuevoLibro);
		}

		void eliminarLibroPorCodigo() {
			string id;
			cout << "Ingrese ID del libro a eliminar: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, id);

			eliminarLibro(id);
		}

		// Menú de gestión de clientes
		void menuClientes() {
			int opcion;
			do {
				cout << "\n--- MENÚ DE GESTIÓN DE CLIENTES ---" << endl;
				cout << "1. Agregar cliente" << endl;
				cout << "2. Mostrar clientes" << endl;
				cout << "3. Actualizar cliente" << endl;
				cout << "4. Volver al menú principal" << endl;
				cout << "Ingrese opción: ";
				cin >> opcion;

				switch (opcion) {
					case 1:
						agregarClienteDesdeInput();
						break;
					case 2:
						mostrarClientes();
						break;
					case 3:
						actualizarClientePorCodigo();
						break;
					case 4:
						cout << "Volviendo al menú principal." << endl;
						break;
					default:
						cout << "Opción inválida. Intente de nuevo." << endl;
						break;
				}
			} while (opcion != 4);
		}

		void agregarClienteDesdeInput() {
			string id, nombre;

			cout << "Ingrese ID del cliente: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, id);
			cout << "Ingrese nombre del cliente: ";
			getline(cin, nombre);

			Cliente cliente(id, nombre);
			agregarCliente(cliente);

			cout << "Cliente agregado correctamente." << endl;
		}

		void actualizarClientePorCodigo() {
			string id;
			cout << "Ingrese ID del cliente a actualizar: ";
			cin.ignore(); // Para limpiar el buffer de entrada
			getline(cin, id);

			// Obtener nuevo nombre del cliente
			string nuevoNombre;
			cout << "Ingrese nuevo nombre del cliente: ";
			getline(cin, nuevoNombre);

			actualizarCliente(id, nuevoNombre);
		}

		// Mostrar préstamos actuales
		void mostrarPrestamos() const {
			cout << "\n--- LISTADO DE PRÉSTAMOS ---" << endl;
			if (prestamos.empty()) {
				cout << "No hay préstamos registrados." << endl;
			} else {
				for (const auto& pair : prestamos) {
					string nombreCliente = findClienteName(pair.first);
					cout << "Cliente: " << nombreCliente << endl;
					for (const auto& libroId : pair.second) {
						string tituloLibro = findLibroTitle(libroId);
						cout << "- Libro: " << tituloLibro << endl;
					}
					cout << endl;
				}
			}
		}

		// Función auxiliar para buscar nombre de cliente por ID
		string findClienteName(const string& idCliente) const {
			auto itCliente = find_if(clientes.begin(), clientes.end(), [&idCliente](const Cliente& cliente) {
				return cliente.getId() == idCliente;
			});

			if (itCliente != clientes.end()) {
				return itCliente->getNombre();
			} else {
				return "Cliente no encontrado";
			}
		}

		// Función auxiliar para buscar título de libro por ID
		string findLibroTitle(const string& idLibro) const {
			auto itLibro = find_if(libros.begin(), libros.end(), [&idLibro](const Libro& libro) {
				return libro.getId() == idLibro;
			});

			if (itLibro != libros.end()) {
				return itLibro->getTitulo();
			} else {
				return "Libro no encontrado";
			}
		}
};

int main() {
	Biblioteca biblioteca;

	// Ejemplo de libros y clientes predefinidos
	biblioteca.agregarLibro(Libro("L001", "Cien años de soledad", "Gabriel García Márquez", 5));
	biblioteca.agregarLibro(Libro("L002", "El principito", "Antoine de Saint-Exupéry", 3));
	biblioteca.agregarCliente(Cliente("C001", "Juan Pérez"));
	biblioteca.agregarCliente(Cliente("C002", "María López"));

	// Menú principal
	biblioteca.menuPrincipal();

	return 0;
}
