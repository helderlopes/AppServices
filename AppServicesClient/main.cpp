#include "pch.h"

using namespace winrt;
using namespace winrt::Windows::ApplicationModel::AppService;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;

IAsyncAction TestAppServiceAsync(hstring appServiceName, hstring packageFamilyName)
{
    AppServiceConnection connection = AppServiceConnection();
    //ensure_close close_connection(connection);

    connection.AppServiceName(appServiceName);
    connection.PackageFamilyName(packageFamilyName);
    AppServiceConnectionStatus status = co_await connection.OpenAsync();

    switch (status)
    {
    case AppServiceConnectionStatus::Success:
        // The new connection opened successfully
        printf("Connection established\n");
        break;
    default:
        printf("Connection failed\n");
        co_return;
    }

    ValueSet inputs;
    inputs.Insert(L"minvalue", box_value(0));
    inputs.Insert(L"maxvalue", box_value(10));
    AppServiceResponse response = co_await connection.SendMessageAsync(inputs);

    if (response.Status() == AppServiceResponseStatus::Success)
    {
        auto result = response.Message().TryLookup(L"result").try_as<int>();
        if (result.has_value())
        {
            printf("value retrieved: %d\n", result.value());
        }
    }
}

static bool TestAppService(hstring appServiceName, hstring packageFamilyName)
{
    AppServiceConnection connection = AppServiceConnection();
    //ensure_close close_connection(connection);

    connection.AppServiceName(appServiceName);
    connection.PackageFamilyName(packageFamilyName);
    AppServiceConnectionStatus status = connection.OpenAsync().get();

    switch (status)
    {
    case AppServiceConnectionStatus::Success:
        // The new connection opened successfully
        printf("Connection established\n");
        break;
    default:
        printf("Connection failed\n");
        return false;
    }

    ValueSet inputs;
    inputs.Insert(L"minvalue", box_value(0));
    inputs.Insert(L"maxvalue", box_value(10));
    AppServiceResponse response = connection.SendMessageAsync(inputs).get();

    if (response.Status() == AppServiceResponseStatus::Success)
    {
        std::optional<int> result = response.Message().TryLookup(L"result").try_as<int>();
        if (result.has_value())
        {
            printf("value retrieved: %d\n", result.value());
            return true;
        }
    }

    return true;
}

int main()
{
    init_apartment();

    hstring appServiceName = L"com.microsoft.randomnumbergenerator";
    hstring packageFamilyName = L"AppServicesProvider_y77qp3c5a60dj";

    TestAppServiceAsync(appServiceName, packageFamilyName).get();
}
