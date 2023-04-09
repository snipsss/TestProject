#pragma once

#include <stdio.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <chrono>
#include "ILS_Logger.h"

//=============================================================================
/// Стандартная реализация большинства методов интерфейса \c ILogger.
/// @ingroup Kernel
/// Данный класс реализует некоторые простым стандартным образом основные 
/// механизмы интрефейса \c ILogger. <i> При реализации механизма регистрации хода процессов, 
/// удобнее наследоваться именно от \c BaseLogger, а не непосредственно от \c ILogger. </i>
/// Данный класс реализует:
/// - формирование заголовка сообщений;
/// - элементарные настройки формата сообщений.
/// \see ILogger , StdLogger
class BaseLogger : public ILogger {
	//---------------------------------------------------------------------------
protected: // Управляющие аттрибуты
	/// Общее количество ошибок зафиксированных этим логгером.
	mutable unsigned int errors = 0;
	/// Общее количество предупреждеий зафиксированных этим логгером.
	mutable unsigned int warnings = 0;
	//---------------------------------------------------------------------------
public: // Конструктор
	BaseLogger();
	//---------------------------------------------------------------------------
public: // Настройки StdLogger-а
	/// Маска битов, определяющая что именно выводить в заголовке.
	///  - 1 выводить дату
	///  - 2 выводить время
	///  - 4 выводить количество секунд с начала
	mutable unsigned int show_info;
	/// Флаг того, что стартовали отсчёт времени
	mutable bool bStarted;
	/// Флаг того, что нужно выводить лог в консоль
	mutable bool bLogToConsole;
	/// Время начала работы.
	mutable std::chrono::steady_clock::time_point start_time;
protected: // Функции интерфейса
	//---------------------------------------------------------------------------
	// Регистрация сообщения для анализатора логов
	virtual void infOut(const Msg& msg, const LogId& id) const;
	// Регистрация информационного сообщения
	virtual void logOut(const Msg& msg, const LogId& id) const;
	// Регистрация предупреждения (warning) и не фатальной ошибки
	virtual void wrnOut(const Msg& msg, const LogId& id) const;
	// Регистрация фатальной ошибки, после которой результаты процесса не определены
	virtual void errOut(const Msg& msg, const LogId& id) const;
protected: // Вспомогательные функции
	/// Создание строки со стандартным заголовком для сообщения.
	/// Создается общая для всех типов сообщений строка с заголовком на основе 
	/// значений настроек.
	/// \note Эту функцию можно переопределить, в случае необходимости 
	/// генерировать заголовок сообщения отличный  от стандартного.
	/// \see log() , wrn() , error() 
	virtual std::string title() const;
	/// Создание строки со стандартным заголовком для информационного сообщения.
	/// Создание строки со стандартным заголовком для информационного сообщения.
	/// \note Эту функцию можно переопределить, в случае необходимости 
	/// генерировать заголовок сообщения отличный  от стандартного.
	virtual std::string lTitle() const;
	/// Создание строки со стандартным заголовком для информационного сообщения.
	/// Создание строки со стандартным заголовком для информационного сообщения.
	/// \note Эту функцию можно переопределить, в случае необходимости 
	/// генерировать заголовок сообщения отличный  от стандартного.
	virtual std::string iTitle() const;
	/// Создание строки со стандартным заголовком для анализатора логов.
	/// Создание строки со стандартным заголовком для анализатора логов.
	/// \note Эту функцию можно переопределить, в случае необходимости 
	/// генерировать заголовок сообщения отличный  от стандартного.
	virtual std::string wTitle() const;
	/// Создание строки со стандартным заголовком для ошибки.
	/// Создание строки со стандартным заголовком для ошибки.
	/// \note Эту функцию можно переопределить, в случае необходимости 
	/// генерировать заголовок сообщения отличный  от стандартного.
	virtual std::string eTitle() const;
	//---------------------------------------------------------------------------
protected: // Функиции механизма вывода
	/// Вывод информационного сообщения.
	/// Вируальная функция, которая <b>должна быть обязательно переопределена</b>
	/// как вывод пользователю информационного сообщения, чтобы происходил 
	/// реальный вывод на экран или в файл или в графическое окно и т.п.
	/// \param msg - текст информационного сообщения.
	virtual void lOut(const std::string& msg) const = 0;
	/// Вывод предупреждения.
	/// Вируальная функция, которая <b>должна быть обязательно переопределена</b>
	/// как вывод пользователю предупреждения, чтобы происходил 
	/// реальный вывод на экран или в файл или в графическое окно и т.п.
	/// \param msg - текст предупреждения.
	virtual void wOut(const std::string& msg) const = 0;
	/// Вывод ошибки.
	/// Вируальная функция, которая <b>должна быть обязательно переопределена</b>
	/// как вывод пользователю обшибки, чтобы происходил 
	/// реальный вывод на экран или в файл или в графическое окно и т.п.
	/// \param msg - текст ошибки.
	virtual void eOut(const std::string& msg) const = 0;
}; //struct BaseLogger

//=============================================================================
/// Реализация регистратора хода процесса (Логгера) для потоков.
/// @ingroup Kernel
/// Реализация регистратора хода процесса (Логгера) для стандартных потоков
/// библиотеки STL. Данный класс наследован от класса BaseLogger и сохраняет
/// всю его функциональность, переопределив только функции вывода сообщений так,
/// чтобы реальный вывод осуществлялся на потоки вывода библиотеки STL переданные
/// в конструкторе данного класса.
/// \see ILogger , BaseLogger
class StdLogger : public BaseLogger {
protected:
	using BaseLogger::bLogToConsole;
	// Потоки для вывода информационных сообщений, предупрежнений и ошибок
	mutable std::ostream *log_out, *wrn_out, *err_out;
	bool l_del,w_del,e_del;
	//---------------------------------------------------------------------------
public: // Конструктор
	/// Конструктор.
	/// Конструировние Логгера на потоки вывода библиотеки STL.
	/// \param l_out - поток для вывода информационных сообщений.
	/// \param w_out - поток для вывода предупреждений.
	/// \param e_out - поток для вывода ошибок.
	StdLogger(std::ostream& l_out=std::cout,
	          std::ostream& w_out=std::cerr,
	          std::ostream& e_out=std::cerr) {
		log_out = &l_out; l_del=false;
		wrn_out = &w_out; w_del=false;
		err_out = &e_out; e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}

	//---------------------------------------------------------------------------
	/// Конструктор по имени файла для инф. сообщений.
	/// Конструировние Логгера на потоки вывода библиотеки STL.
	/// \param l_out_file - Имя файла для вывода log информации.
	/// \param w_out - поток для вывода предупреждений.
	/// \param e_out - поток для вывода ошибок.
	/// 
	/// \note Если в качестве имени файла (первого параметра) передается "", 
	/// это значит, что информационные сообщения должны ингорироваться, а выводится 
	/// только предупреждения и ошибки.
	StdLogger(std::string l_out_file,
	          std::ostream& w_out=std::cerr,
	          std::ostream& e_out=std::cerr,
			  std::ios_base::openmode mode = std::ios_base::out
			  ) {
		using namespace std;
		log_out = NULL;l_del=false;;
		if(l_out_file!="") {
			std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(),mode|std::ios_base::out);
			if(*log_stream) {log_out = log_stream;l_del=true;}
			else delete log_stream;
		}
		wrn_out = &w_out; w_del=false;
		err_out = &e_out; e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	//---------------------------------------------------------------------------
	/// Конструктор по имени файла для инф. сообщений и предупрждений.
	/// Конструировние Логгера на потоки вывода библиотеки STL.
	/// \param l_out_file - Имя файла для вывода log информации.
	/// \param w_out_file - Имя файла для вывода предупреждений.
	/// \param e_out - поток для вывода ошибок.
	/// 
	/// \note Если в качестве имени файла передается "", 
	/// это значит, что сообщения такого типа должны ингорироваться, а выводится 
	/// будут только другие сообщения.
	StdLogger(std::string l_out_file,
	          std::string w_out_file,
	          std::ostream& e_out=std::cerr,
			  std::ios_base::openmode mode = std::ios_base::out
			  ) {
		using namespace std;
		log_out = NULL;l_del=false;;
		if(l_out_file!="") {
			std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(),mode|std::ios_base::out);
			if(*log_stream) {log_out = log_stream;l_del=true;}
			else delete log_stream;
		}
		wrn_out = NULL;w_del=false;;
		if(w_out_file!="") {
			std::ofstream* wrn_stream = new std::ofstream(w_out_file.c_str(),mode|std::ios_base::out);
			if(*wrn_stream) {wrn_out = wrn_stream;w_del=true;}
			else delete wrn_stream;
		}
		err_out = &e_out; e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	//---------------------------------------------------------------------------
	/// Конструктор по имени файла для всех сообщений.
	/// Конструировние Логгера на потоки вывода библиотеки STL.
	/// \param l_out_file - Имя файла для вывода log информации.
	/// \param w_out_file - Имя файла для вывода предупреждений.
	/// \param e_out_file - Имя файла для вывода ошибок.
	/// 
	/// \note Если в качестве имени файла передается "", 
	/// это значит, что сообщения такого типа должны ингорироваться, а выводится 
	/// будут только другие сообщения.
	StdLogger(std::string l_out_file,
	          std::string w_out_file,
	          std::string e_out_file,
			  std::ios_base::openmode mode = std::ios_base::out
			  ) {
		using namespace std;
		log_out = NULL;l_del=false;;
		if(l_out_file!="") {
			std::ofstream* log_stream = new std::ofstream(l_out_file.c_str(),mode|std::ios_base::out);
			if(*log_stream) {log_out = log_stream;l_del=true;}
			else delete log_stream;
		}
		wrn_out = NULL;w_del=false;;
		if(w_out_file==l_out_file && w_out_file!="") {
			wrn_out = log_out;
			w_del = false;
		} else if(w_out_file!="") {
			std::ofstream* wrn_stream = new std::ofstream(w_out_file.c_str(),mode|std::ios_base::out);
			if(*wrn_stream) {wrn_out = wrn_stream;w_del=true;}
			else delete wrn_stream;
		}
		err_out = NULL;e_del=false;;
		if(e_out_file==l_out_file && e_out_file!="") {
			err_out = log_out;
			e_del = false;
		} else if(e_out_file==w_out_file && e_out_file!="") {
			err_out = wrn_out;
			e_del = false;
		} else if(e_out_file!="") {
			std::ofstream* err_stream = new std::ofstream(e_out_file.c_str(),mode|std::ios_base::out);
			if(*err_stream) {err_out = err_stream;e_del=true;}
			else delete err_stream;
		}
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	//---------------------------------------------------------------------------
	/// Конструктор копирования.
	StdLogger(const StdLogger& src) {
		if(src.l_del) log_out=NULL; // не можем прокопировать, иначе возникнет ситуация когда объект родитель умрет и будем иметь указатель на невалидный объект
		else log_out = src.log_out;
		if(src.w_del) wrn_out=NULL; // не можем прокопировать, иначе возникнет ситуация когда объект родитель умрет и будем иметь указатель на невалидный объект
		else wrn_out = src.wrn_out;
		if(src.e_del) err_out=NULL; // не можем прокопировать, иначе возникнет ситуация когда объект родитель умрет и будем иметь указатель на невалидный объект
		else err_out = src.err_out;
		l_del=false;
		w_del=false;
		e_del=false;
		BaseLogger::onLogStart(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
	}
	//---------------------------------------------------------------------------
	virtual ~StdLogger() {
		BaseLogger::onLogFinish(true,wrn_out!=log_out,err_out!=wrn_out&&err_out!=log_out);
		if(l_del && log_out) delete log_out;
		if(w_del && wrn_out) delete wrn_out;
		if(e_del && err_out) delete err_out;
	}
	//---------------------------------------------------------------------------
protected: // Функиции механизма вывода
	virtual void ConsoleOut(const std::string& msg) const {
		if (bLogToConsole)
			std::cout << msg << std::endl;
	}
	virtual void lOut(const std::string& msg) const {
		if (log_out) (*log_out) << msg << std::endl;
		ConsoleOut(msg);
	}
	virtual void wOut(const std::string& msg) const {
		if (wrn_out) (*wrn_out) << msg << std::endl;
		ConsoleOut(msg);
	}
	virtual void eOut(const std::string& msg) const {
		if (err_out) (*err_out) << msg << std::endl;
		ConsoleOut(msg);
	}
}; //struct StdLogger
