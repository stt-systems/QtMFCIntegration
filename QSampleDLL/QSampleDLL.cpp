// +
#include "QSampleDLL.h"
#include "../QtMFCIntegration/QtMFCIntegration.h"
#include "ui_Dialog.h"
#include <qtranslator.h>

using namespace STT::QtMFCIntegration;

namespace
{
  /**
   Automates the removable of the translator from the application on destruction.
   */
  struct QTranslatorDestructor
  {
    void operator()(QTranslator *ptr) const
    {
      if (qApp) { qApp->removeTranslator(ptr); }
      delete ptr;
    }
  };

  /**
   A helper unique pointer to `QTranslator`.
   */
  using QTranslatorUnique = std::unique_ptr<QTranslator, QTranslatorDestructor>;

  auto createDelegateWrapper(QSampleDLL::Delegate *delegate)
  {
    return STT::QtMFCIntegration::QtDialogsDelegateWrapper{delegate, &QSampleDLL::Delegate::myDialogWillBeShown,
                                                           &QSampleDLL::Delegate::myDialogHasBeenClosed};
  }
}

struct QSampleDLL::Private
{
  QString title;

  QString locale{"en"};
  QTranslatorUnique translator;

  QString text;
  int number{0};
};

QSampleDLL::QSampleDLL(const std::string &title) : p{new Private{}}
{
  p->title = QString::fromStdString(title);
}

QSampleDLL::~QSampleDLL()
{
  delete p;
}

void QSampleDLL::setLocale(const std::string &locale)
{
  p->locale = QString::fromStdString(locale);

  // This example only uses the ISO 639 language code
  p->locale = p->locale.left(p->locale.indexOf("_"));
}

void QSampleDLL::setText(const std::string &text) noexcept
{
  p->text = QString::fromStdString(text);
}

std::string QSampleDLL::getText() const noexcept
{
  return p->text.toStdString();
}

std::wstring QSampleDLL::getWText() const noexcept
{
  return p->text.toStdWString();
}

void QSampleDLL::setNumber(int number) noexcept
{
  p->number = number;
}

int QSampleDLL::getNumber() const noexcept
{
  return p->number;
}

bool QSampleDLL::showDialog(Delegate *delegate)
{
  testAndCreateQtApplication();

  // Install translators for current locale
  p->translator.reset();
  QLocale::setDefault(QLocale{p->locale});

  if (QTranslatorUnique translator{new QTranslator{}}; translator->load(":/QSampleDLL/translations.qm")) {
    if (qApp->installTranslator(translator.get())) { p->translator.swap(translator); }
  }

  // Common delegate
  auto delegate_wrapper = createDelegateWrapper(delegate);

  // Setup dialog
  QDialog dlg;
  Ui::Dialog ui;
  ui.setupUi(&dlg);

  dlg.setWindowTitle(p->title);

  // Set values
  ui.txtSampleText->setText(p->text);
  ui.spnSampleNumber->setValue(p->number);

  // Connections
  QObject::connect(ui.btnShowMessageBox, &QPushButton::pressed, [&dlg]() {
    QMessageBox::information(&dlg, "", QObject::tr("This is a sample message box.", "QSampleDLL"));
  });

  // This widget
  ui.wdgMFC->create([&dlg]() {
    STT::QtMFCIntegration::showMessageBox(&dlg, "", QMessageBox::Information, QObject::tr("Invoked from MFC button."));
  });

  { // This block is just to emphasize where the delegate's protection takes place
    // Protection magic occurs inside `execDialog`

    if (!execDialog(&dlg, &delegate_wrapper)) { return false; }

    // Anything below this line is not protected by the delegate
  }

  // Retrieve values before destroying the dialog
  p->text = ui.txtSampleText->text();
  p->number = ui.spnSampleNumber->value();

  return true;
}

void QSampleDLL::showMessageBox(Delegate *delegate)
{
  testAndCreateQtApplication();

  const auto message = QObject::tr("Text: %1\nNumber: %2", "QSampleDLL").arg(p->text).arg(p->number);

  auto delegate_wrapper = createDelegateWrapper(delegate);
  STT::QtMFCIntegration::showMessageBox(nullptr, "", QMessageBox::Information, message, QMessageBox::Ok,
                                        QMessageBox::Ok, &delegate_wrapper);
}

void QSampleDLL::activateWindow()
{
  if (auto current_dialog = getActiveDialog()) { current_dialog->activateWindow(); }
}
