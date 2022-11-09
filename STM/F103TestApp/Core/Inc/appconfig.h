/*
 * appconfig.h
 *
 *  Created on: Nov 7, 2022
 *      Author: VertexNi
 */

#ifndef APPCONFIG_H_
#define APPCONFIG_H_

class AppConfig
{
    int _delay;

    AppConfig() = default;
public:
    AppConfig(const AppConfig &rhs) = delete;
    AppConfig &operator=(const AppConfig &rhs) = delete;
    static AppConfig& getInstance()
    {
        static AppConfig _instance;
    	return _instance;
    }

    int getDelay(){return _delay;}
    void setDelay(int delay)
    {
        if (delay > 0)
        {
            _delay = delay;
        }
    }
};

#endif /* APPCONFIG_H_ */
