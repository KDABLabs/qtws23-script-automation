#include "historymodel.h"
#include "logger.h"

HistoryModel::HistoryModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    LoggerObject::m_model = this;
}

HistoryModel::~HistoryModel()
{
    LoggerObject::m_model = nullptr;
}

int HistoryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_data.size());
}

int HistoryModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

static QString variantToString(const QVariant &variant)
{
    QString text = variant.toString();
    if (static_cast<QMetaType::Type>(variant.typeId()) == QMetaType::QString) {
        text.replace('\\', R"(\\)");
        text.replace('\n', R"(\n)");
        text.replace('\t', R"(\t)");
        text.replace('"', R"(\")");
        text.append('"');
        text.prepend('"');
    } else if (variant.metaType().flags().testAnyFlag(QMetaType::IsEnumeration)) {
        QString className = variant.metaType().metaObject()->className();
        className = className.split("::").last();
        text = className + '.' + text;
    }
    return text;
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(checkIndex(index, CheckIndexOption::IndexIsValid));

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case NameCol:
            return m_data.at(index.row()).name;
        case ParamCol: {
            const auto &params = m_data.at(index.row()).params;
            QStringList paramStrings;
            for (const auto &param : params) {
                QString text = variantToString(param.value);
                if (!param.name.isEmpty())
                    text.prepend(QString("%1: ").arg(param.name));
                paramStrings.push_back(text);
            }
            const QString returnVariable = m_data.at(index.row()).returnArg.name;
            return paramStrings.join(", ") + (returnVariable.isEmpty() ? "" : (" => " + returnVariable));
        }
        }
    }
    return {};
}

QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole)
        return {};

    switch (section) {
    case NameCol:
        return tr("API name");
    case ParamCol:
        return tr("Parameters");
    }
    return {};
}

void HistoryModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

QString HistoryModel::createScript(int start, int end)
{
    std::tie(start, end) = std::minmax(start, end);
    Q_ASSERT(start >= 0 && start <= end && end < static_cast<int>(m_data.size()));

    QString scriptText = "// Description of the script\n\n";

    QHash<QString, QVariant> returnVariables;

    for (int row = start; row <= end; ++row) {
        const auto &data = m_data.at(row);
        QString apiCall = data.name;
        const bool isProperty = m_properties.contains(apiCall);
        apiCall.replace("::", ".");

        // Set the return value
        QString returnValue;
        if (!data.returnArg.isEmpty()) {
            const auto &name = data.returnArg.name;
            returnValue = (returnVariables.contains(name) ? "" : "let ") + name + " = ";
            returnVariables[name] = data.returnArg.value;
        }

        // Pass the parameters
        QStringList paramStrings;
        for (const auto &param : data.params) {
            if (!param.name.isEmpty() && returnVariables.value(param.name) == param.value) {
                paramStrings.push_back(param.name);
                continue;
            }

            QString text = variantToString(param.value);
            paramStrings.push_back(text);
        }

        if (isProperty) {
            if (paramStrings.isEmpty())
                scriptText += returnValue + apiCall + '\n';
            else
                scriptText += returnValue + QString("%1 = %2\n").arg(apiCall, paramStrings.first());
        } else {
            scriptText += returnValue + QString("%1(%2)\n").arg(apiCall, paramStrings.join(", "));
        }
    }

    return scriptText;
}

QString HistoryModel::createScript(const QModelIndex &startIndex, const QModelIndex &endIndex)
{
    Q_ASSERT(checkIndex(startIndex, CheckIndexOption::IndexIsValid)
             && checkIndex(endIndex, CheckIndexOption::IndexIsValid));
    return createScript(startIndex.row(), endIndex.row());
}

void HistoryModel::addData(LogData &&data, bool merge)
{
    if (!merge || m_data.empty() || m_data.back().name != data.name) {
        beginInsertRows({}, static_cast<int>(m_data.size()), static_cast<int>(m_data.size()));
        m_data.push_back(std::move(data));
        endInsertRows();
        return;
    }

    auto &lastData = m_data.back();
    // Add parameters together
    for (size_t i = 0; i < data.params.size(); ++i) {
        const auto &param = data.params[i];
        auto &lastParam = lastData.params[i];
        switch (static_cast<QMetaType::Type>(param.value.typeId())) {
        case QMetaType::Int:
            lastParam.value = lastParam.value.toInt() + param.value.toInt();
            break;
        case QMetaType::QString:
            lastParam.value = lastParam.value.toString() + param.value.toString();
            break;
        case QMetaType::QStringList:
            lastParam.value = lastParam.value.toStringList() + param.value.toStringList();
            break;
        default:
            Q_UNREACHABLE();
        }
    }
    auto lastIndex = index(static_cast<int>(m_data.size()) - 1, ParamCol);
    emit dataChanged(lastIndex, lastIndex);
}
