/*
    ChibiOS - Copyright (C) 2014-2015 Fabio Utzig

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    SPIv1/hal_spi_lld.h
 * @brief   KINETIS SPI subsystem low level driver header.
 * @author  andreika <prometheus.pcb@gmail.com>
 *
 * @addtogroup SPI
 * @{
 */

#ifndef HAL_SPI_LLD_H_
#define HAL_SPI_LLD_H_

#if HAL_USE_SPI || defined(__DOXYGEN__)

#include "fsl_common.h"
#include "fsl_lpspi.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Circular mode support flag.
 */
#define SPI_SUPPORTS_CIRCULAR           TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SPI0 driver enable switch.
 * @details If set to @p TRUE the support for SPI0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_SPI_USE_SPI0) || defined(__DOXYGEN__)
#define KINETIS_SPI_USE_SPI0                  FALSE
#endif

/**
 * @brief   SPI0 interrupt priority level setting.
 */
#if !defined(KINETIS_SPI_SPI0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define KINETIS_SPI_SPI0_IRQ_PRIORITY         10
#endif

/**
 * @brief   SPI1 driver enable switch.
 * @details If set to @p TRUE the support for SPI0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(KINETIS_SPI_USE_SPI1) || defined(__DOXYGEN__)
#define KINETIS_SPI_USE_SPI1                  FALSE
#endif

/**
 * @brief   SPI1 interrupt priority level setting.
 */
#if !defined(KINETIS_SPI_SPI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define KINETIS_SPI_SPI1_IRQ_PRIORITY         10
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if KINETIS_SPI_USE_SPI0 && !KINETIS_HAS_SPI0
#error "SPI0 not present in the selected device"
#endif

#if KINETIS_SPI_USE_SPI1 && !KINETIS_HAS_SPI1
#error "SPI1 not present in the selected device"
#endif

#if KINETIS_SPI_USE_SPI0 && KINETIS_SPI_USE_SPI1
#error "Only one SPI peripheral can be enabled"
#endif

#if !(KINETIS_SPI_USE_SPI0 || KINETIS_SPI_USE_SPI1)
#error "SPI driver activated but no SPI peripheral assigned"
#endif

#if KINETIS_SPI_USE_SPI0 &&                                                 \
    !OSAL_IRQ_IS_VALID_PRIORITY(KINETIS_SPI_SPI0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI0"
#endif

#if KINETIS_SPI_USE_SPI1 &&                                                 \
    !OSAL_IRQ_IS_VALID_PRIORITY(KINETIS_SPI_SPI1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to SPI1"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an SPI driver.
 */
typedef struct SPIDriver SPIDriver;

/**
 * @brief   SPI notification callback type.
 *
 * @param[in] spip      pointer to the @p SPIDriver object triggering the
 *                      callback
 */
typedef void (*spicallback_t)(SPIDriver *spip);

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Enables the circular buffer mode.
   */
  bool                      circular;
#endif
  /**
   * @brief Operation complete callback or @p NULL.
   */
  spicallback_t             end_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief The chip select line port - when not using pcs.
   */
  ioportid_t                ssport;
  /**
   * @brief The chip select line pad number - when not using pcs.
   */
  uint16_t                  sspad;
  /**
   * @brief SPI CR1 register initialization data.
   */
  uint16_t                  cr1;
  /**
   * @brief SPI CR2 register initialization data.
   */
  uint16_t                  cr2;
} SPIConfig;

/**
 * @brief   Structure representing a SPI driver.
 */
struct SPIDriver {
  /**
   * @brief Driver state.
   */
  spistate_t                state;
  /**
   * @brief Current configuration data.
   */
  const SPIConfig           *config;
#if SPI_USE_WAIT || defined(__DOXYGEN__)
  /**
   * @brief Waiting thread.
   */
  thread_reference_t        thread;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
  /**
   * @brief Mutex protecting the bus.
   */
  mutex_t                   mutex;
#endif /* SPI_USE_MUTUAL_EXCLUSION */
#if defined(SPI_DRIVER_EXT_FIELDS)
  SPI_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/

  /**
   * @brief Pointer to the SPIx registers block.
   */
  LPSPI_Type               *spi;

  /**
   * @brief Master or Slave.
   */
  bool isMaster;

  /**
   * @brief Stores the data for SPI master transfer.
   */
  lpspi_transfer_t handleXfer;
  
  /**
   * @brief Transfer flags (including kLPSPI_MasterPcs*) for this SPI config.
   */
  int32_t flags;

  /**
   * @brief IRQ callback handle for SPI master non-blocking transfer.
   */
  lpspi_master_handle_t masterHandle;
  /**
   * @brief IRQ callback handle for SPI slave non-blocking transfer.
   */
  lpspi_slave_handle_t slaveHandle;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/* TAR settings for n bits at SYSCLK / 2 */
#define KINETIS_SPI_TAR_SYSCLK_DIV_2(n)\
    SPIx_CTARn_FMSZ((n) - 1) | \
    SPIx_CTARn_CPOL | \
    SPIx_CTARn_CPHA | \
    SPIx_CTARn_DBR | \
    SPIx_CTARn_PBR(0) | \
    SPIx_CTARn_BR(0) | \
    SPIx_CTARn_CSSCK(0) | \
    SPIx_CTARn_ASC(0) | \
    SPIx_CTARn_DT(0)

/* TAR settings for n bits at SYSCLK / 4096 for debugging */
#define KINETIS_SPI_TAR_SYSCLK_DIV_4096(n) \
    SPIx_CTARn_FMSZ(((n) - 1)) | \
    SPIx_CTARn_CPOL | \
    SPIx_CTARn_CPHA | \
    SPIx_CTARn_PBR(0) | \
    SPIx_CTARn_BR(0xB) | \
    SPIx_CTARn_CSSCK(0xB) | \
    SPIx_CTARn_ASC(0x7) | \
    SPIx_CTARn_DT(0xB)

#define KINETIS_SPI_TAR_8BIT_FAST   KINETIS_SPI_TAR_SYSCLK_DIV_2(8)
#define KINETIS_SPI_TAR_8BIT_SLOW   KINETIS_SPI_TAR_SYSCLK_DIV_4096(8)

#define KINETIS_SPI_TAR0_DEFAULT    KINETIS_SPI_TAR_SYSCLK_DIV_2(8)
#define KINETIS_SPI_TAR1_DEFAULT    KINETIS_SPI_TAR_SYSCLK_DIV_2(8)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if KINETIS_SPI_USE_SPI0 && !defined(__DOXYGEN__)
extern SPIDriver SPID1;
#endif

#if KINETIS_SPI_USE_SPI1 && !defined(__DOXYGEN__)
extern SPIDriver SPID2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void spi_lld_init(void);
  void spi_lld_start(SPIDriver *spip);
  void spi_lld_stop(SPIDriver *spip);
  void spi_lld_select(SPIDriver *spip);
  void spi_lld_unselect(SPIDriver *spip);
  void spi_lld_ignore(SPIDriver *spip, size_t n);
  void spi_lld_exchange(SPIDriver *spip, size_t n,
                        const void *txbuf, void *rxbuf);
  void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf);
  void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf);
#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
  void spi_lld_abort(SPIDriver *spip);
#endif
  uint16_t spi_lld_polled_exchange(SPIDriver *spip, uint16_t frame);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SPI */

#endif /* HAL_SPI_LLD_H_ */

/** @} */
