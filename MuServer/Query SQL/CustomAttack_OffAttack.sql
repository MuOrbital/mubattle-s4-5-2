USE [MuOnline]
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

ALTER PROCEDURE [dbo].[WZ_GetAccountLevel]
	@Account varchar(10)
AS
BEGIN
	SET NOCOUNT ON
	SET XACT_ABORT ON

	DECLARE @CurrentAccountLevel int
	DECLARE @CurrentAccountExpireDate smalldatetime

	SELECT
		@CurrentAccountLevel = AccountLevel,
		@CurrentAccountExpireDate = AccountExpireDate
	FROM MEMB_INFO
	WHERE memb___id = @Account

	IF (@CurrentAccountLevel <> 0 AND @CurrentAccountExpireDate IS NOT NULL AND GETDATE() > @CurrentAccountExpireDate)
	BEGIN
		SET @CurrentAccountLevel = 0

		UPDATE MEMB_INFO
		SET AccountLevel = 0
		WHERE memb___id = @Account
	END

	SELECT
		@CurrentAccountLevel AS AccountLevel,
		@CurrentAccountExpireDate AS AccountExpireDate

	SET NOCOUNT OFF
	SET XACT_ABORT OFF
END
GO

IF OBJECT_ID('dbo.CustomAttack', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[CustomAttack]
	(
		[Account] varchar(10) NOT NULL CONSTRAINT [DF_CustomAttack_Account] DEFAULT(''),
		[Name] varchar(10) NOT NULL CONSTRAINT [DF_CustomAttack_Name] DEFAULT(''),
		[Active] int NOT NULL CONSTRAINT [DF_CustomAttack_Active] DEFAULT(0),
		[Skill] int NOT NULL CONSTRAINT [DF_CustomAttack_Skill] DEFAULT(0),
		[Map] int NOT NULL CONSTRAINT [DF_CustomAttack_Map] DEFAULT(0),
		[PosX] int NOT NULL CONSTRAINT [DF_CustomAttack_PosX] DEFAULT(0),
		[PosY] int NOT NULL CONSTRAINT [DF_CustomAttack_PosY] DEFAULT(0)
	)
END
GO

IF COL_LENGTH('dbo.CustomAttack', 'Account') IS NULL
BEGIN
	ALTER TABLE [dbo].[CustomAttack]
	ADD [Account] varchar(10) NOT NULL CONSTRAINT [DF_CustomAttack_Account] DEFAULT('')
END
GO

UPDATE A
SET A.Account = C.AccountID
FROM [dbo].[CustomAttack] A
INNER JOIN [dbo].[Character] C ON C.Name = A.Name
WHERE ISNULL(A.Account, '') = ''
GO

IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = 'IX_CustomAttack_Account' AND object_id = OBJECT_ID('dbo.CustomAttack'))
BEGIN
	CREATE NONCLUSTERED INDEX [IX_CustomAttack_Account] ON [dbo].[CustomAttack] ([Account])
END
GO

IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = 'IX_CustomAttack_Name' AND object_id = OBJECT_ID('dbo.CustomAttack'))
BEGIN
	CREATE NONCLUSTERED INDEX [IX_CustomAttack_Name] ON [dbo].[CustomAttack] ([Name])
END
GO

IF OBJECT_ID('dbo.CustomOffAttack', 'U') IS NULL
BEGIN
	CREATE TABLE [dbo].[CustomOffAttack]
	(
		[Account] varchar(10) NOT NULL,
		[Name] varchar(10) NOT NULL,
		[Active] int NOT NULL CONSTRAINT [DF_CustomOffAttack_Active] DEFAULT(0),
		[Skill] int NOT NULL CONSTRAINT [DF_CustomOffAttack_Skill] DEFAULT(0),
		[Map] int NOT NULL CONSTRAINT [DF_CustomOffAttack_Map] DEFAULT(0),
		[PosX] int NOT NULL CONSTRAINT [DF_CustomOffAttack_PosX] DEFAULT(0),
		[PosY] int NOT NULL CONSTRAINT [DF_CustomOffAttack_PosY] DEFAULT(0),
		[AutoBuff] int NOT NULL CONSTRAINT [DF_CustomOffAttack_AutoBuff] DEFAULT(0),
		[AccountLevel] int NOT NULL CONSTRAINT [DF_CustomOffAttack_AccountLevel] DEFAULT(0),
		[AccountExpireDate] varchar(20) NOT NULL CONSTRAINT [DF_CustomOffAttack_AccountExpireDate] DEFAULT(''),
		[UpdateDate] datetime NOT NULL CONSTRAINT [DF_CustomOffAttack_UpdateDate] DEFAULT(GETDATE())
	)
END
GO

IF COL_LENGTH('dbo.CustomOffAttack', 'AutoBuff') IS NULL
	ALTER TABLE [dbo].[CustomOffAttack] ADD [AutoBuff] int NOT NULL CONSTRAINT [DF_CustomOffAttack_AutoBuff] DEFAULT(0)
GO

IF COL_LENGTH('dbo.CustomOffAttack', 'AccountLevel') IS NULL
	ALTER TABLE [dbo].[CustomOffAttack] ADD [AccountLevel] int NOT NULL CONSTRAINT [DF_CustomOffAttack_AccountLevel] DEFAULT(0)
GO

IF COL_LENGTH('dbo.CustomOffAttack', 'AccountExpireDate') IS NULL
	ALTER TABLE [dbo].[CustomOffAttack] ADD [AccountExpireDate] varchar(20) NOT NULL CONSTRAINT [DF_CustomOffAttack_AccountExpireDate] DEFAULT('')
GO

IF COL_LENGTH('dbo.CustomOffAttack', 'UpdateDate') IS NULL
	ALTER TABLE [dbo].[CustomOffAttack] ADD [UpdateDate] datetime NOT NULL CONSTRAINT [DF_CustomOffAttack_UpdateDate] DEFAULT(GETDATE())
GO

IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = 'IX_CustomOffAttack_Account' AND object_id = OBJECT_ID('dbo.CustomOffAttack'))
BEGIN
	CREATE NONCLUSTERED INDEX [IX_CustomOffAttack_Account] ON [dbo].[CustomOffAttack] ([Account])
END
GO

IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = 'IX_CustomOffAttack_Name' AND object_id = OBJECT_ID('dbo.CustomOffAttack'))
BEGIN
	CREATE NONCLUSTERED INDEX [IX_CustomOffAttack_Name] ON [dbo].[CustomOffAttack] ([Name])
END
GO

UPDATE O
SET Active = 0,
	UpdateDate = GETDATE()
FROM [dbo].[CustomOffAttack] O
INNER JOIN MEMB_INFO M ON M.memb___id = O.Account
WHERE O.Active = 1
AND (
	(M.AccountLevel <> 0 AND M.AccountExpireDate IS NOT NULL AND GETDATE() > M.AccountExpireDate)
	OR
	(M.AccountLevel = 0 AND O.AccountLevel <> 0)
)
GO

UPDATE MEMB_INFO
SET AccountLevel = 0
WHERE AccountLevel <> 0
AND AccountExpireDate IS NOT NULL
AND GETDATE() > AccountExpireDate
GO
