#ifndef SINGLETON_H
#define SINGLETON_H

#include <iostream>
#include <QObject>
template <typename T>
class Singleton : public QObject
{
protected:
    Singleton(){ }
    virtual ~Singleton(){ destroyObject(); }

    void destroyObject() {
        onDestroyed();
    }

    virtual void onDestroyed() = 0;
public:
    static T *getInstance()
    {
        if(m_singleton == NULL)
        {
            m_singleton = new T;
            m_singleton->onFirstInit();
        }
        return (static_cast<T*> (m_singleton));
    }

    /**
     * @brief This method is called the first time the class is allocated
     */
    virtual void onFirstInit() {

    }

    static void kill()
    {
        if(m_singleton != NULL)
        {
            delete m_singleton;
            m_singleton = NULL;
        }
    }

private:
    //Instance unique
    static T *m_singleton;
};

template <typename T>
T *Singleton<T>::m_singleton = NULL;

#endif // SINGLETON_H
