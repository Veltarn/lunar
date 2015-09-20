#ifndef LUNARPLUGINSINTERFACE_H
#define LUNARPLUGINSINTERFACE_H

class LunarPluginsInterface
{
public:
    virtual ~LunarPluginsInterface() {}
    /**
     * @brief onLoad is called when the plugin is loaded by the plugin manager
     */
    virtual void onLoad() = 0;
    /**
     * @brief onUnload is called when the plugin is unloaded by the plugin manager
     */
    virtual void onUnload() = 0;

};

#define LunarPluginsInterface_IID "net.veltarn.LunarPluginsInterface"

Q_DECLARE_INTERFACE(LunarPluginsInterface, LunarPluginsInterface_IID)
#endif // LUNARPLUGINSINTERFACE_H
