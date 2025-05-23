#include "Model.h"
#include <QDir>
#include <QFile>
#include <QTextStream>

Model::Model(){
}

Model::~Model(){

}

void Model::mainThread(){
    this->processes.clear();
    this->readProc();
}

void Model::readProc(){
    QDir dir(PROC_PATH);
    QStringList processes = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot  | QDir::Readable, QDir::Name);
    for (const QString &process : processes) {
        dir.cd(process);
        process_t proc;

        QFile statFile(dir.filePath("stat"));
        if (statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&statFile);
            QString statContent = in.readAll();
            parseProcStat(proc, statContent);
            statFile.close();
        }

        this->processes.push_back(proc);
        dir.cdUp();
    }
}

void Model::parseProcStat(process_t& proc, QString content){
    QStringList tokens = content.split(QChar::Space);
    proc.pid = tokens[0].toInt();
    proc.name = tokens[1].mid(1, tokens[1].length() - 2).toStdString();
    proc.cpu = tokens[38].toInt();
    proc.ppid = tokens[3].toInt();
    proc.state = tokens[2].toStdString();
    proc.total_memory = tokens[22].toLongLong();
    proc.ram_memory = tokens[23].toLongLong();
}
