#pragma once

#include <QAbstractTableModel>

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

private:
    friend class LoggerObject;

    struct Arg
    {
        QVariant value;
    };
    struct LogData
    {
        QString name;
        std::vector<Arg> params;
    };

    void logData(const QString &name) { addData(LogData {name, {}}); }
    template <typename... Ts>
    void logData(const QString &name, Ts... params)
    {
        LogData data;
        data.name = name;
        fillLogData(data, params...);
        addData(std::move(data));
    }

    void fillLogData(LogData &) {};

    template <typename T, typename... Ts>
    void fillLogData(LogData &data, T param, Ts... params)
    {
        data.params.push_back({QVariant::fromValue(param)});

        fillLogData(data, params...);
    }

    void addData(LogData &&data);

    std::vector<LogData> m_data;
};
