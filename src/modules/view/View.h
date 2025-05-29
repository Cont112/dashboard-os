/**
 * @file View.h
 * @brief Header para a classe View
 * @author Pedro Neves
 */

#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QQmlApplicationEngine>
#include "../model/Model.h"

/**
 * @brief Classe que implementa a interface de usuário do dashboard
 * @details Essa classe é responsável por carregar o arquivo QML e exibir a interface do dashboard
 */
class View : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Construtor da classe View
     * @param model Ponteiro para o modelo de dados
     * @param parent Ponteiro para o objeto pai (opcional)
     */
    View(Model* model, QObject *parent = nullptr);

    ~View();
    
private:
    QQmlApplicationEngine engine;
};

#endif