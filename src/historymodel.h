#pragma once

#include <QAbstractTableModel>
#include <QSet>

struct LoggerArgBase
{
};
/**
 * @brief Argument for a call
 * The argName will be matched to an existing returned value from a previous method, when recording a script.
 * If empty, or not set by a previous method, the value will be used.
 */
template <typename T>
struct LoggerArg : public LoggerArgBase
{
    LoggerArg(QString &&name, T v)
        : argName(name)
        , value(v)
    {
    }
    QString argName;
    T value;
    QString toString() const { return valueToString(value); }
};

class HistoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns { NameCol = 0, ParamCol, ColumnCount };

    explicit HistoryModel(QObject *parent = nullptr);
    ~HistoryModel();

    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void clear();

    /**
     * @brief Create a script from 2 points in the history
     * The script is created using 2 rows in the history model. It will create a javascript script.
     */
    QString createScript(int start, int end);
    QString createScript(const QModelIndex &startIndex, const QModelIndex &endIndex);

    template <typename Object>
    static void addProperties()
    {
        for (int i = 0; i < Object::staticMetaObject.propertyCount(); ++i) {
            QString className = Object::staticMetaObject.className();
            m_properties.insert(
                QString("%1::%2").arg(className.split("::").last()).arg(Object::staticMetaObject.property(i).name()));
        }
    }

private:
    friend class LoggerObject;

    struct Arg
    {
        QString name;
        QVariant value;
        bool isEmpty() const { return name.isEmpty(); }
    };
    struct LogData
    {
        QString name;
        std::vector<Arg> params;
        Arg returnArg;
    };

    void logData(const QString &name) { addData(LogData {name, {}}, false); }
    template <typename... Ts>
    void logData(const QString &name, bool merge, Ts... params)
    {
        LogData data;
        data.name = name;
        fillLogData(data, params...);
        addData(std::move(data), merge);
    }

    template <typename T>
    void setReturnValue(QString &&name, const T &value)
    {
        m_data.back().returnArg.name = std::move(name);
        m_data.back().returnArg.value = QVariant::fromValue(value);
    }

    void fillLogData(LogData &) {};

    template <typename T, typename... Ts>
    void fillLogData(LogData &data, T param, Ts... params)
    {
        if constexpr (std::derived_from<T, LoggerArgBase>)
            data.params.push_back({param.argName, QVariant::fromValue(param.value)});
        else
            data.params.push_back({"", QVariant::fromValue(param)});

        fillLogData(data, params...);
    }

    void addData(LogData &&data, bool merge);

    std::vector<LogData> m_data;
    inline static QSet<QString> m_properties = {};
};
